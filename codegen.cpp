#include "codegen.h"

#include "ast.h"
#include "semantic.h"

#include <cassert>
#include <unordered_map>
#include <vector>
#include <string>

namespace COGEN{ /* START NAMESPACE */

static const std::unordered_map<std::string, std::string> l_predefinedVariableRegisterName = {
    {"gl_FragColor", "result.color"},
    {"gl_FragDepth", "result.depth"},
    {"gl_FragCoord", "fragment.position"},
    {"gl_TexCoord", "fragment.texcoord"},
    {"gl_Color", "fragment.color"},
    {"gl_Secondary", "fragment.color.secondary"},
    {"gl_FogFragCoord", "fragment.fogcoord"},
    {"gl_Light_Half", "state.light[0].half"},
    {"gl_Light_Ambient", "state.lightmodel.ambient"},
    {"gl_Material_Shininess", "state.material.shininess"},
    {"env1", "program.env[1]"},
    {"env2", "program.env[2]"},
    {"env3", "program.env[3]"}
};

std::string getPredefinedVariableRegisterName(const std::string &variableName) {
    auto fit = l_predefinedVariableRegisterName.find(variableName);
    if(fit == l_predefinedVariableRegisterName.end()) {
        assert(0);
        return "";
    }
    return fit->second;
}


std::string getRegisterIndexing(unsigned index) {
    switch(index) {
        case 0: return "x";
        case 1: return "y";
        case 2: return "z";
        case 3: return "w";
        default:
            assert(0);
    }
    return "";
}


/* Stores the ARB Assembly */
class ARBAssemblyDatabase {
    private:
        class TempRegDeclaration {
            private:
                std::string m_regName;
            
            public:
                TempRegDeclaration(const std::string &regName):
                    m_regName(regName) {}

            public:
                const std::string &getRegName() const {return m_regName; }
        };

        class ParamRegDeclaration {
            private:
                std::string m_regName;
                std::string m_regValue;
            
            public:
                ParamRegDeclaration(const std::string &regName, const std::string &regValue):
                    m_regName(regName), m_regValue(regValue) {}

            public:
                const std::string &getRegName() const { return m_regName; }
                const std::string &getRegValue() const { return m_regValue; }
        };

    private:
        /* For user-defined variables */
        std::vector<TempRegDeclaration> m_userTempRegDeclarations;
        std::vector<ParamRegDeclaration> m_userParamRegDeclarations;

        /* For auto-generated variables, e.g. immediate and intermediate values */
        std::vector<TempRegDeclaration> m_autoTempRegDeclarations;
        unsigned m_autoTempRegDeclarationsSessionCount = 0;
        std::vector<ParamRegDeclaration> m_autoParamRegDeclarations;

    public:
        void declareUserTempRegister(const std::string &regName) {
            m_userTempRegDeclarations.emplace_back(regName);
        }

        void declareUserParamRegister(const std::string &regName, const std::string &regValue) {
            m_userParamRegDeclarations.emplace_back(regName, regValue);
        }
    
    public:
        void print() const {
            unsigned lineNumber = 0;

            for(const auto &tempDecl: m_userTempRegDeclarations) {
                lineNumber++;
                printf("%u: TEMP %s;\n", lineNumber, tempDecl.getRegName().c_str());
            }

            for(const auto &paramDecl: m_userParamRegDeclarations) {
                lineNumber++;
                printf("%u: PARAM %s = %s;\n", lineNumber, paramDecl.getRegName().c_str(), paramDecl.getRegValue().c_str());
            }

            for(const auto &tempDecl: m_autoTempRegDeclarations) {
                lineNumber++;
                printf("%u: TEMP %s;\n", lineNumber, tempDecl.getRegName().c_str());
            }

            for(const auto &paramDecl: m_autoParamRegDeclarations) {
                lineNumber++;
                printf("%u: PARAM %s = %s;\n", lineNumber, paramDecl.getRegName().c_str(), paramDecl.getRegValue().c_str());
            }
        }
};


/* Flattened symbol table with resolved symbol names */
class DeclaredSymbolRegisterTable {
    public:
        using NameToDeclHashTable = std::unordered_map<std::string, const AST::DeclarationNode *>;
        using DeclToNameHashTable = std::unordered_map<const AST::DeclarationNode *, std::string>;
    
    private:
        NameToDeclHashTable m_regNameToDecl;
        DeclToNameHashTable m_declToregName;
    
    public:
        bool hasRegisterName(const std::string &regName) const {
            return (m_regNameToDecl.count(regName) == 1);
        }

        bool hasDeclaration(const AST::DeclarationNode *decl) const {
            return (m_declToregName.count(decl) == 1);
        }

        const std::string &getRegisterName(const AST::DeclarationNode *decl) const {
            auto fit = m_declToregName.find(decl);
            assert(fit != m_declToregName.end());
            return fit->second;
        }

        void insert(const std::string &regName, const AST::DeclarationNode *decl) {
            assert(!hasRegisterName(regName));
            assert(!hasDeclaration(decl));

            m_regNameToDecl.emplace(regName, decl);
            m_declToregName.emplace(decl, regName);

            assert(m_regNameToDecl.size() == m_declToregName.size());
        }
    
    public:
        const NameToDeclHashTable &getRegNameToDeclMapping()const { return m_regNameToDecl; }
        const DeclToNameHashTable &getDeclToregNameMapping()const { return m_declToregName; }

    public:
        void print() const {
            for(const auto &p : m_regNameToDecl) {
                printf("%s : ", p.first.c_str());
                ast_print(const_cast<AST::DeclarationNode *>(p.second));
                printf("\n");
            }
        }
};


class SymbolDeclVisitor: public AST::Visitor {
    private:
        static const std::string s_symbolNamePrefix;
    
    private:
        DeclaredSymbolRegisterTable m_declaredSymbolRegisterTable;

    private:
        SymbolDeclVisitor() = default;

    private:
        virtual void preNodeVisit(AST::DeclarationNode *declarationNode) {
            if(!declarationNode->isOrdinaryType()) {
                m_declaredSymbolRegisterTable.insert(getPredefinedVariableRegisterName(declarationNode->getName()), declarationNode);
                return;
            }

            std::string symbolDeclaredName = declarationNode->getName();
            std::string symbolRegisterName = s_symbolNamePrefix + symbolDeclaredName;

            std::string symbolRegisterResolvedName = symbolRegisterName;
            
            // avoid same symbol declaration name
            unsigned duplicate_count = 0;
            do{
                symbolRegisterResolvedName = symbolRegisterName + "_" + std::to_string(duplicate_count);
                duplicate_count++;
            }while(m_declaredSymbolRegisterTable.hasRegisterName(symbolRegisterResolvedName));

            m_declaredSymbolRegisterTable.insert(symbolRegisterResolvedName, declarationNode);
        }

    public:
        static DeclaredSymbolRegisterTable getDeclaredSymbolRegisterTable(AST::ASTNode *astNode) {
            SymbolDeclVisitor symbolDeclVisitor;
            astNode->visit(symbolDeclVisitor);

            return symbolDeclVisitor.m_declaredSymbolRegisterTable;
        }
};
const std::string SymbolDeclVisitor::s_symbolNamePrefix = "_$";


class DeclaredSymbolRegisterCodeGenerator: public AST::Visitor {
    private:
        const DeclaredSymbolRegisterTable &m_declaredSymbolRegisterTable;

        std::string m_assemblyValue; // for const qualifed values
        bool m_successful = true;

    private:
        DeclaredSymbolRegisterCodeGenerator(const DeclaredSymbolRegisterTable &declaredSymbolRegisterTable):
            m_declaredSymbolRegisterTable(declaredSymbolRegisterTable) {}

    private:
        virtual void preNodeVisit(AST::ExpressionsNode *expressionsNode){
            m_assemblyValue += "{";
        }

    private:
        virtual void postNodeVisit(AST::ExpressionsNode *expressionsNode){
            m_assemblyValue += "}";
        }

        virtual void postNodeVisit(AST::UnaryExpressionNode *unaryExpressionNode){
            m_successful = false;
        }

        virtual void postNodeVisit(AST::BinaryExpressionNode *binaryExpressionNode){
            m_successful = false;
        }

        virtual void postNodeVisit(AST::IntLiteralNode *intLiteralNode){
            m_assemblyValue += std::to_string(static_cast<float>(intLiteralNode->getVal()));
        }

        virtual void postNodeVisit(AST::FloatLiteralNode *floatLiteralNode){
            m_assemblyValue += std::to_string(floatLiteralNode->getVal());
        }

        virtual void postNodeVisit(AST::BooleanLiteralNode *booleanLiteralNode){
            m_assemblyValue += std::to_string(static_cast<float>(booleanLiteralNode->getVal()));
        }

        virtual void postNodeVisit(AST::IdentifierNode *identifierNode){
            const AST::DeclarationNode *decl = identifierNode->getDeclaration();
            if(decl->isOrdinaryType()) {
                if(!decl->isConst()) {
                    m_successful = false;
                    return;
                }

                // const ordinary type
                const AST::DeclarationNode *decl = identifierNode->getDeclaration();
                AST::ExpressionNode *initExpr = (decl->getInitValue()) ? decl->getInitValue(): decl->getExpression();
                initExpr->visit(*this);
            } else {
                // predefined variable
                m_assemblyValue += m_declaredSymbolRegisterTable.getRegisterName(identifierNode->getDeclaration());
            }
        }
        
        virtual void postNodeVisit(AST::IndexingNode *indexingNode){
            m_assemblyValue += "." +
                getRegisterIndexing(dynamic_cast<AST::IntLiteralNode *>(indexingNode->getIndexExpression())->getVal());
        }
        
        virtual void postNodeVisit(AST::FunctionNode *functionNode){
            m_successful = false;
        }

    private:
        virtual void nodeVisit(AST::ExpressionsNode *expressionsNode) {
            int counter = 0;
            for(AST::ExpressionNode *expr: expressionsNode->getExpressionList()) {
                if(counter != 0) {
                    m_assemblyValue += ",";
                }
                expr->visit(*this);
                counter++;
            }
        }
        virtual void nodeVisit(AST::IndexingNode *indexingNode) {
            indexingNode->getIdentifier()->visit(*this);
        }
        virtual void nodeVisit(AST::FunctionNode *functionNode) {}

    private:
        static bool generateConstQualifiedValue(const DeclaredSymbolRegisterTable &declaredSymbolRegisterTable, AST::ExpressionNode *expr, std::string &assemblyLine) {
            DeclaredSymbolRegisterCodeGenerator visitor(declaredSymbolRegisterTable);
            expr->visit(visitor);
            assemblyLine = std::move(visitor.m_assemblyValue);
            return visitor.m_successful;
        }

    public:
        static void generateCode(const DeclaredSymbolRegisterTable &declaredSymbolRegisterTable, ARBAssemblyDatabase &assemblyDB) {
            const DeclaredSymbolRegisterTable::NameToDeclHashTable &regToDecl =
                declaredSymbolRegisterTable.getRegNameToDeclMapping();
            
            for(const auto &p: regToDecl) {
                const AST::DeclarationNode *decl = p.second;
                if(decl->isOrdinaryType()) {
                    if(decl->isConst()) {
                        AST::ExpressionNode *initExpr = (decl->getInitValue()) ? decl->getInitValue(): decl->getExpression();

                        std::string initialValueAssemblyInstruction;
                        if(!generateConstQualifiedValue(declaredSymbolRegisterTable, initExpr, initialValueAssemblyInstruction)) {
                            initialValueAssemblyInstruction = "{";
                            int dataTypeOrder = SEMA::getDataTypeOrder(decl->getType());
                            for(int i = 0; i < dataTypeOrder; i++) {
                                if(i != 0) {
                                    initialValueAssemblyInstruction += ",";
                                }
                                initialValueAssemblyInstruction += "0.0";
                            }
                            initialValueAssemblyInstruction += "}";
                        }
                        assemblyDB.declareUserParamRegister(p.first, initialValueAssemblyInstruction);
                    } else {
                        assemblyDB.declareUserTempRegister(p.first);
                    }
                }
            }
        }
};

} /* END NAMESPACE */


int genCode(node *ast) {
    COGEN::ARBAssemblyDatabase assemblyDB;

    COGEN::DeclaredSymbolRegisterTable declaredSymbolRegisterTable =
        COGEN::SymbolDeclVisitor::getDeclaredSymbolRegisterTable(static_cast<AST::ASTNode *>(ast));

    printf("\n");
    printf("Declared Symbol Register Table\n");
    declaredSymbolRegisterTable.print();

    COGEN::DeclaredSymbolRegisterCodeGenerator::generateCode(declaredSymbolRegisterTable, assemblyDB);

    printf("\n");
    printf("ARB Assembly Database\n");
    assemblyDB.print();
    
    return 0;
}