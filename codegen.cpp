#include "codegen.h"

#include "ast.h"
#include "semantic.h"
#include "common.h"
#include "parser.tab.h"

#include <cassert>
#include <unordered_map>
#include <vector>
#include <string>


/*
    ARB Assembly Guideline:
    https://www.khronos.org/registry/OpenGL/extensions/ARB/ARB_fragment_program.txt
*/
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
                TempRegDeclaration(const std::string &regName): m_regName(regName) {}
                TempRegDeclaration(std::string &&regName): m_regName(regName) {}

            public:
                const std::string &getRegName() const { return m_regName; }
                std::string generateCode() const { return "TEMP " + m_regName + ";"; }
        };

        class ParamRegDeclaration {
            private:
                std::string m_regName;
                std::string m_regValue;
            
            public:
                ParamRegDeclaration(const std::string &regName, const std::string &regValue): m_regName(regName), m_regValue(regValue) {}
                ParamRegDeclaration(std::string &&regName, std::string &&regValue): m_regName(std::move(regName)), m_regValue(std::move(regValue)) {}

            public:
                const std::string &getRegName() const { return m_regName; }
                const std::string &getRegValue() const { return m_regValue; }
                std::string generateCode() const { return "PARAM " + m_regName + " = " + m_regValue + ";"; }
        };

    public:
        // CMP v,v,v v compare
        // MOV v v move
        // ADD v,v v add
        // SUB v,v v subtract
        // MUL v,v v multiply
        // RCP s ssss reciprocal
        // POW s,s ssss exponentiate
        // DP3 v,v ssss 3-component dot product
        // LIT v v compute light coefficients
        // RSQ s ssss reciprocal square root
        // MAX v,v v maximum
        // MIN v,v v minimum
        // ABS v v absolute value
        enum class OPCode {
            CMP,
            MOV,
            ADD,
            SUB,
            MUL,
            RCP,
            POW,
            DP3,
            LIT,
            RSQ,
            MAX,
            MIN,
            ABS
        };

    private:
        class Instruction {
            private:
                OPCode m_OPCode;
                std::string m_out;
                std::string m_in0;
                std::string m_in1;
                std::string m_in2;

            public:
                Instruction(OPCode opCode, const std::string &out, const std::string &in0, std::string in1 = "", std::string in2 = ""):
                    m_OPCode(opCode), m_out(out), m_in0(in0), m_in1(in1), m_in2(in2) {}
            
            public:
                std::string generateCode() const {
                    std::string operatorString;
                    switch(m_OPCode) {
                        case OPCode::ADD:
                            return std::string("ADD") + " " + m_out + ", " + m_in0 + ", " + m_in1;
                        case OPCode::SUB:
                            return std::string("SUB") + " " + m_out + ", " + m_in0 + ", " + m_in1;
                        case OPCode::MUL:
                            return std::string("MUL") + " " + m_out + ", " + m_in0 + ", " + m_in1;
                        case OPCode::POW:
                            return std::string("POW") + " " + m_out + ", " + m_in0 + ", " + m_in1;
                        
                        case OPCode::RCP:
                            return std::string("RCP") + " " + m_out + ", " + m_in0;

                        case OPCode::MOV:
                            return std::string("MOV") + " " + m_out + ", " + m_in0;

                        case OPCode::CMP:
                            return std::string("CMP") + " " + m_out + ", " + m_in0 + ", " + m_in1 + ", " + m_in2;

                        case OPCode::DP3:
                            return std::string("DP3") + " " + m_out + ", " + m_in0 + ", " + m_in1;
                        case OPCode::LIT:
                            return std::string("LIT") + " " + m_out + ", " + m_in0;
                        case OPCode::RSQ:
                            return std::string("RSQ") + " " + m_out + ", " + m_in0;

                        case OPCode::MAX:
                            return std::string("MAX") + " " + m_out + ", " + m_in0 + ", " + m_in1;
                        case OPCode::MIN:
                            return std::string("MIN") + " " + m_out + ", " + m_in0 + ", " + m_in1;

                        case OPCode::ABS:
                            return std::string("ABS") + " " + m_out + ", " + m_in0;

                        default:
                            assert(0);
                            return "";
                    }
                }
        };

    private:
        /* For user-defined variables */
        std::vector<TempRegDeclaration> m_userTempRegDeclarations;
        std::vector<ParamRegDeclaration> m_userParamRegDeclarations;

        /* For auto-generated intermediate variables */
        std::vector<TempRegDeclaration> m_autoTempRegDeclarations;
        unsigned m_autoTempRegDeclarationsSessionCount = 0;
        /* For auto-generated long-live intermediate variables */
        std::vector<TempRegDeclaration> m_autoLongLiveTempRegDeclarations;
        /* For auto-generated immediate values */
        std::vector<ParamRegDeclaration> m_autoParamRegDeclarations;

        /* For assembly instructions */
        std::vector<Instruction> m_instructions;

    public:
        void declareUserTempRegister(const std::string &regName) {
            m_userTempRegDeclarations.emplace_back(regName);
        }

        void declareUserParamRegister(const std::string &regName, const std::string &regValue) {
            m_userParamRegDeclarations.emplace_back(regName, regValue);
        }

    public:
        void newAutoTempRegisterAllocationSession() { m_autoTempRegDeclarationsSessionCount = 0; }

        const std::string &requestAutoTempRegister() {
            if(m_autoTempRegDeclarationsSessionCount < m_autoTempRegDeclarations.size()) {
                return m_autoTempRegDeclarations[m_autoTempRegDeclarationsSessionCount++].getRegName();
            }
            assert(m_autoTempRegDeclarationsSessionCount == m_autoTempRegDeclarations.size());
            m_autoTempRegDeclarations.emplace_back("__$temp_" + std::to_string(m_autoTempRegDeclarationsSessionCount++));
            assert(m_autoTempRegDeclarationsSessionCount == m_autoTempRegDeclarations.size());

            return m_autoTempRegDeclarations.back().getRegName();
        }

        const std::string &requestLongLiveAutoTempRegister() {
            unsigned count = m_autoLongLiveTempRegDeclarations.size();
            m_autoLongLiveTempRegDeclarations.emplace_back("__$templl_" + std::to_string(count));

            return m_autoLongLiveTempRegDeclarations.back().getRegName();
        }

        const std::string &requestAutoParamRegister(const std::string &regValue) {
            unsigned count = m_autoParamRegDeclarations.size();
            m_autoParamRegDeclarations.emplace_back("__$param_" + std::to_string(count), regValue);

            return m_autoParamRegDeclarations.back().getRegName();
        }

    public:
        void insertInstruction(OPCode opCode, const std::string &out, const std::string &in0, std::string in1 = "", std::string in2 = "") {
            m_instructions.emplace_back(opCode, out, in0, in1, in2);
        }
    
    public:
        void print() const {
            std::vector<std::string> assemblyCode;


            assemblyCode.emplace_back("# User Declared Non-Constant Variables");
            for(const auto &tempDecl: m_userTempRegDeclarations) {
                assemblyCode.push_back(tempDecl.generateCode());
            }
            assemblyCode.emplace_back("");


            assemblyCode.emplace_back("");
            assemblyCode.emplace_back("# User Declared Constant Variables");
            for(const auto &paramDecl: m_userParamRegDeclarations) {
                assemblyCode.push_back(paramDecl.generateCode());
            }
            assemblyCode.emplace_back("");


            assemblyCode.emplace_back("");
            assemblyCode.emplace_back("# Auto-Generated Re-usable Intermediate Value Registers");
            for(const auto &tempDecl: m_autoTempRegDeclarations) {
                assemblyCode.push_back(tempDecl.generateCode());
            }
            assemblyCode.emplace_back("");


            assemblyCode.emplace_back("");
            assemblyCode.emplace_back("# Auto-Generated Non-reusable Intermediate Value Registers");
            for(const auto &tempDecl: m_autoLongLiveTempRegDeclarations) {
                assemblyCode.push_back(tempDecl.generateCode());
            }
            assemblyCode.emplace_back("");


            assemblyCode.emplace_back("");
            assemblyCode.emplace_back("# Auto-Generated Immediate Value Registers");
            for(const auto &paramDecl: m_autoParamRegDeclarations) {
                assemblyCode.push_back(paramDecl.generateCode());
            }
            assemblyCode.emplace_back("");


            assemblyCode.emplace_back("");
            assemblyCode.emplace_back("# Instructions");
            for(const auto &ins: m_instructions) {
                assemblyCode.push_back(ins.generateCode());
            }
            assemblyCode.emplace_back("");


            for(unsigned i = 0; i < assemblyCode.size(); i++) {
                printf("%-12u %s\n", i + 1, assemblyCode[i].c_str());
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

        void sendToAssemblyDB(ARBAssemblyDatabase &assemblyDB) const;
};


DeclaredSymbolRegisterTable createDeclaredSymbolRegisterTable(AST::ASTNode *astNode) {
    class SymbolDeclVisitor: public AST::Visitor {
        private:
            const char *m_symbolNamePrefix = "$";
        
        public:
            DeclaredSymbolRegisterTable m_declaredSymbolRegisterTable;

        private:
            virtual void preNodeVisit(AST::DeclarationNode *declarationNode) {
                if(!declarationNode->isOrdinaryType()) {
                    m_declaredSymbolRegisterTable.insert(getPredefinedVariableRegisterName(declarationNode->getName()), declarationNode);
                    return;
                }

                std::string symbolDeclaredName = declarationNode->getName();
                std::string symbolRegisterName = m_symbolNamePrefix + symbolDeclaredName;

                std::string symbolRegisterResolvedName = symbolRegisterName;
                
                // avoid same symbol declaration name
                unsigned duplicate_count = 0;
                do{
                    symbolRegisterResolvedName = symbolRegisterName + "_" + std::to_string(duplicate_count);
                    duplicate_count++;
                }while(m_declaredSymbolRegisterTable.hasRegisterName(symbolRegisterResolvedName));

                m_declaredSymbolRegisterTable.insert(symbolRegisterResolvedName, declarationNode);
            }
    };

    SymbolDeclVisitor symbolDeclVisitor;
    astNode->visit(symbolDeclVisitor);

    return symbolDeclVisitor.m_declaredSymbolRegisterTable;
}


class ConstQualifiedExpressionReducer: public AST::Visitor {
    private:
        const DeclaredSymbolRegisterTable &m_declaredSymbolRegisterTable;

        std::string m_assemblyValue; // for const qualifed values
        bool m_successful = true;

    private:
        ConstQualifiedExpressionReducer(const DeclaredSymbolRegisterTable &declaredSymbolRegisterTable):
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
            m_assemblyValue += std::to_string(booleanLiteralNode->getVal() ? 1.0 : -1.0);
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

    public:
        /* Reduce the expression to value */
        static std::string reduceToValue(const DeclaredSymbolRegisterTable &declaredSymbolRegisterTable, AST::ExpressionNode *expr) {
            ConstQualifiedExpressionReducer reducer(declaredSymbolRegisterTable);
            expr->visit(reducer);
            if(reducer.m_successful) {
                return reducer.m_assemblyValue;
            } else {
                std::string assemblyValue = "{";
                int dataTypeOrder = SEMA::getDataTypeOrder(expr->getExpressionType());
                for(int i = 0; i < dataTypeOrder; i++) {
                    if(i != 0) {
                        assemblyValue += ",";
                    }
                    assemblyValue += "0.0";
                }
                assemblyValue += "}";
                return assemblyValue;
            }
        }
};


class ExpressionReducer: public AST::Visitor {
    private:
        const DeclaredSymbolRegisterTable &m_declaredSymbolRegisterTable;
        ARBAssemblyDatabase &m_assemblyDB;

        std::string m_resultRegName;

    private:
        ExpressionReducer(const DeclaredSymbolRegisterTable &declaredSymbolRegisterTable, ARBAssemblyDatabase &assemblyDB):
            m_declaredSymbolRegisterTable(declaredSymbolRegisterTable), m_assemblyDB(assemblyDB) {}
        
    private:
        /* Disable traversal, we do reduction here */
        virtual void nodeVisit(AST::UnaryExpressionNode *unaryExpressionNode);
        virtual void nodeVisit(AST::BinaryExpressionNode *binaryExpressionNode);
        virtual void nodeVisit(AST::IntLiteralNode *intLiteralNode);
        virtual void nodeVisit(AST::FloatLiteralNode *floatLiteralNode);
        virtual void nodeVisit(AST::BooleanLiteralNode *booleanLiteralNode);
        virtual void nodeVisit(AST::IdentifierNode *identifierNode);
        virtual void nodeVisit(AST::IndexingNode *indexingNode);
        virtual void nodeVisit(AST::FunctionNode *functionNode);
        virtual void nodeVisit(AST::ConstructorNode *constructorNode);

    
    public:
        /* Reduce the expression into a register containing the expression result */
        static std::string reduce(const DeclaredSymbolRegisterTable &declaredSymbolRegisterTable,
            ARBAssemblyDatabase &assemblyDB,
            AST::ExpressionNode *expr);
};

void ExpressionReducer::nodeVisit(AST::UnaryExpressionNode *unaryExpressionNode) {
    std::string rhsRegName = reduce(m_declaredSymbolRegisterTable,
        m_assemblyDB,
        unaryExpressionNode->getExpression());
    
    m_resultRegName = m_assemblyDB.requestAutoTempRegister();

    assert(unaryExpressionNode->getOperator() == MINUS || unaryExpressionNode->getOperator() == NOT);
    m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::MOV, m_resultRegName, "-" + rhsRegName);
}

void ExpressionReducer::nodeVisit(AST::BinaryExpressionNode *binaryExpressionNode) {
    std::string lhsRegName = reduce(m_declaredSymbolRegisterTable,
    m_assemblyDB,
    binaryExpressionNode->getLeftExpression());

    std::string rhsRegName = reduce(m_declaredSymbolRegisterTable,
    m_assemblyDB,
    binaryExpressionNode->getRightExpression());

    m_resultRegName = m_assemblyDB.requestAutoTempRegister();

    switch(binaryExpressionNode->getOperator()) {
        case AND:
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::MIN, m_resultRegName, lhsRegName, rhsRegName);
            break;
        case OR:
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::MAX, m_resultRegName, lhsRegName, rhsRegName);
            break;
        case PLUS:
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::ADD, m_resultRegName, lhsRegName, rhsRegName);
            break;
        case MINUS:
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::SUB, m_resultRegName, lhsRegName, rhsRegName);
            break;
        case TIMES:
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::MUL, m_resultRegName, lhsRegName, rhsRegName);
            break;
        case SLASH: {
            std::string rcpResult = m_assemblyDB.requestAutoTempRegister();
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::RCP, rcpResult, rhsRegName);
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::MUL, m_resultRegName, lhsRegName, rcpResult);
            break;
        }
        case EXP:
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::POW, m_resultRegName, lhsRegName, rhsRegName);
            break;
        case EQL: {
            std::string tempReg = m_assemblyDB.requestAutoTempRegister();
            // represent their difference as negative number
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::SUB, tempReg, lhsRegName, rhsRegName);
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::ABS, tempReg, tempReg);
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::MOV, tempReg, "-" + tempReg);
            
            // component-wise eql comparison
            std::string trueReg = m_assemblyDB.requestAutoParamRegister("{1.0,1.0,1.0,1.0}");
            std::string falseReg = m_assemblyDB.requestAutoParamRegister("{-1.0,-1.0,-1.0,-1.0}");
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::CMP, tempReg, tempReg, falseReg, trueReg);

            // and them up
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::MIN, tempReg + ".x", tempReg + ".x", tempReg + ".y");
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::MIN, tempReg + ".x", tempReg + ".x", tempReg + ".z");
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::MIN, tempReg + ".x", tempReg + ".x", tempReg + ".w");

            // store result
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::MOV, m_resultRegName, tempReg);

            break;
        }
        case NEQ: { // negate of EQL
            std::string tempReg = m_assemblyDB.requestAutoTempRegister();
            // represent their difference as negative number
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::SUB, tempReg, lhsRegName, rhsRegName);
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::ABS, tempReg, tempReg);
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::MOV, tempReg, "-" + tempReg);
            
            // component-wise eql comparison
            std::string trueReg = m_assemblyDB.requestAutoParamRegister("{1.0,1.0,1.0,1.0}");
            std::string falseReg = m_assemblyDB.requestAutoParamRegister("{-1.0,-1.0,-1.0,-1.0}");
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::CMP, tempReg, tempReg, falseReg, trueReg);

            // and them up
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::MIN, tempReg + ".x", tempReg + ".x", tempReg + ".y");
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::MIN, tempReg + ".x", tempReg + ".x", tempReg + ".z");
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::MIN, tempReg + ".x", tempReg + ".x", tempReg + ".w");

            // store result, negate the EQL operation
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::MOV, m_resultRegName, "-" + tempReg);

            break;
        }
        case LSS: {
            std::string tempReg = m_assemblyDB.requestAutoTempRegister();
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::SUB, tempReg, lhsRegName, rhsRegName);
            // < 0 if lss
            std::string trueReg = m_assemblyDB.requestAutoParamRegister("{1.0,1.0,1.0,1.0}");
            std::string falseReg = m_assemblyDB.requestAutoParamRegister("{-1.0,-1.0,-1.0,-1.0}");
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::CMP, m_resultRegName, tempReg, trueReg, falseReg);

            break;
        }
        case GEQ: { // negate of LSS
            std::string tempReg = m_assemblyDB.requestAutoTempRegister();
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::SUB, tempReg, lhsRegName, rhsRegName);
            // < 0 if lss
            std::string trueReg = m_assemblyDB.requestAutoParamRegister("{1.0,1.0,1.0,1.0}");
            std::string falseReg = m_assemblyDB.requestAutoParamRegister("{-1.0,-1.0,-1.0,-1.0}");
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::CMP, m_resultRegName, tempReg, falseReg, trueReg);

            break;
        }
        case GTR: {
            std::string tempReg = m_assemblyDB.requestAutoTempRegister();
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::SUB, tempReg, rhsRegName, lhsRegName);
            // < 0 if gtr
            std::string trueReg = m_assemblyDB.requestAutoParamRegister("{1.0,1.0,1.0,1.0}");
            std::string falseReg = m_assemblyDB.requestAutoParamRegister("{-1.0,-1.0,-1.0,-1.0}");
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::CMP, m_resultRegName, tempReg, trueReg, falseReg);

            break;
        }
        case LEQ: { // negate of GTR
            std::string tempReg = m_assemblyDB.requestAutoTempRegister();
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::SUB, tempReg, rhsRegName, lhsRegName);
            // < 0 if gtr
            std::string trueReg = m_assemblyDB.requestAutoParamRegister("{1.0,1.0,1.0,1.0}");
            std::string falseReg = m_assemblyDB.requestAutoParamRegister("{-1.0,-1.0,-1.0,-1.0}");
            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::CMP, m_resultRegName, tempReg, falseReg, trueReg);

            break;
        }
        default:
            assert(0);
    }
}

void ExpressionReducer::nodeVisit(AST::IntLiteralNode *intLiteralNode) {
    m_resultRegName = m_assemblyDB.requestAutoParamRegister(
        ConstQualifiedExpressionReducer::reduceToValue(m_declaredSymbolRegisterTable, intLiteralNode));
}

void ExpressionReducer::nodeVisit(AST::FloatLiteralNode *floatLiteralNode) {
    m_resultRegName = m_assemblyDB.requestAutoParamRegister(
        ConstQualifiedExpressionReducer::reduceToValue(m_declaredSymbolRegisterTable, floatLiteralNode));
}

void ExpressionReducer::nodeVisit(AST::BooleanLiteralNode *booleanLiteralNode) {
    m_resultRegName = m_assemblyDB.requestAutoParamRegister(
        ConstQualifiedExpressionReducer::reduceToValue(m_declaredSymbolRegisterTable, booleanLiteralNode));
}

void ExpressionReducer::nodeVisit(AST::IdentifierNode *identifierNode) {
    m_resultRegName = m_declaredSymbolRegisterTable.getRegisterName(identifierNode->getDeclaration());
}

void ExpressionReducer::nodeVisit(AST::IndexingNode *indexingNode) {
    m_resultRegName = m_declaredSymbolRegisterTable.getRegisterName(indexingNode->getDeclaration());
    m_resultRegName += "." +
                getRegisterIndexing(dynamic_cast<AST::IntLiteralNode *>(indexingNode->getIndexExpression())->getVal());
}

void ExpressionReducer::nodeVisit(AST::FunctionNode *functionNode) {
    const std::string &funcName = functionNode->getName();
    AST::ExpressionsNode *exprs = functionNode->getArgumentExpressions();
    if(funcName == "rsq") {
        m_resultRegName = m_assemblyDB.requestAutoTempRegister();
        assert(exprs->getNumberExpression() == 1);
        std::string arg1RegName = reduce(m_declaredSymbolRegisterTable, m_assemblyDB, exprs->getExpressionAt(0));

        m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::RSQ, m_resultRegName, arg1RegName);
    } else if (funcName == "dp3") {
        m_resultRegName = m_assemblyDB.requestAutoTempRegister();
        assert(exprs->getNumberExpression() == 2);
        std::string arg1RegName = reduce(m_declaredSymbolRegisterTable, m_assemblyDB, exprs->getExpressionAt(0));
        std::string arg2RegName = reduce(m_declaredSymbolRegisterTable, m_assemblyDB, exprs->getExpressionAt(1));

        m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::DP3, m_resultRegName, arg1RegName, arg2RegName);
    } else if (funcName == "lit") {
        m_resultRegName = m_assemblyDB.requestAutoTempRegister();
        assert(exprs->getNumberExpression() == 1);
        std::string arg1RegName = reduce(m_declaredSymbolRegisterTable, m_assemblyDB, exprs->getExpressionAt(0));

        m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::LIT, m_resultRegName, arg1RegName);
    } else {
        assert(0);
    }
}

void ExpressionReducer::nodeVisit(AST::ConstructorNode *constructorNode) {
    m_resultRegName = m_assemblyDB.requestAutoParamRegister(
        ConstQualifiedExpressionReducer::reduceToValue(m_declaredSymbolRegisterTable, constructorNode));
}

std::string ExpressionReducer::reduce(const DeclaredSymbolRegisterTable &declaredSymbolRegisterTable,
            ARBAssemblyDatabase &assemblyDB,
            AST::ExpressionNode *expr) {
    ExpressionReducer reducer(declaredSymbolRegisterTable, assemblyDB);
    expr->visit(reducer);
    return reducer.m_resultRegName;
}


void sendInstructionToAssemblyDB(ARBAssemblyDatabase &assemblyDB, const DeclaredSymbolRegisterTable &declaredSymbolRegisterTable, AST::ASTNode *ast) {
    class AssignmentVisitor: public AST::Visitor {
        private:
            const DeclaredSymbolRegisterTable &m_declaredSymbolRegisterTable;
            ARBAssemblyDatabase &m_assemblyDB;

            std::string m_currentConditionReg;
            int m_ifScopeCount = 0;

        public:
            AssignmentVisitor(const DeclaredSymbolRegisterTable &declaredSymbolRegisterTable, ARBAssemblyDatabase &assemblyDB):
                m_declaredSymbolRegisterTable(declaredSymbolRegisterTable), m_assemblyDB(assemblyDB) {}

        private:
            /* Disable traversal for expressions */
            virtual void nodeVisit(AST::ExpressionNode *expressionNode) {}
            virtual void nodeVisit(AST::ExpressionsNode *expressionsNode) {}
            virtual void nodeVisit(AST::UnaryExpressionNode *unaryExpressionNode) {}
            virtual void nodeVisit(AST::BinaryExpressionNode *binaryExpressionNode) {}
            virtual void nodeVisit(AST::IntLiteralNode *intLiteralNode) {}
            virtual void nodeVisit(AST::FloatLiteralNode *floatLiteralNode) {}
            virtual void nodeVisit(AST::BooleanLiteralNode *booleanLiteralNode) {}
            virtual void nodeVisit(AST::VariableNode *variableNode) {}
            virtual void nodeVisit(AST::IdentifierNode *identifierNode) {}
            virtual void nodeVisit(AST::IndexingNode *indexingNode) {}
            virtual void nodeVisit(AST::FunctionNode *functionNode) {}
            virtual void nodeVisit(AST::ConstructorNode *constructorNode) {}

            virtual void nodeVisit(AST::StatementsNode *statementsNode) {
                for(AST::StatementNode *stmt: statementsNode->getStatementList()) {
                    m_assemblyDB.newAutoTempRegisterAllocationSession();
                    stmt->visit(*this);
                }
            }

            virtual void preNodeVisit(AST::ScopeNode *scopeNode) {
                m_currentConditionReg = m_assemblyDB.requestAutoParamRegister("{1.0,1.0,1.0,1.0}");
            }

            virtual void nodeVisit(AST::IfStatementNode *ifStatementNode) {
                m_ifScopeCount++;

                // save previous
                std::string previousConditionReg = std::move(m_currentConditionReg);

                std::string condReg = ExpressionReducer::reduce(m_declaredSymbolRegisterTable, m_assemblyDB,
                    ifStatementNode->getConditionExpression());
                std::string ownedCondReg = m_assemblyDB.requestAutoTempRegister();
                // convert scalar condition to vector condition
                m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::MOV, ownedCondReg + ".x", condReg + ".x");
                m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::MOV, ownedCondReg + ".y", condReg + ".x");
                m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::MOV, ownedCondReg + ".z", condReg + ".x");
                m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::MOV, ownedCondReg + ".w", condReg + ".x");

                m_currentConditionReg = m_assemblyDB.requestLongLiveAutoTempRegister();

                if((m_ifScopeCount - 1) == 0) {
                    m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::MOV,
                        m_currentConditionReg,
                        ownedCondReg);
                } else {
                    // conditionally set the condition reg
                    m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::CMP,
                        m_currentConditionReg,
                        previousConditionReg,
                        previousConditionReg,       // if outer condition is false, propagate false
                        ownedCondReg                // if outer condition is true, set new condition
                        );
                }

                ifStatementNode->getThenStatement()->visit(*this);

                if(ifStatementNode->getElseStatement() != nullptr) {
                    if((m_ifScopeCount - 1) == 0) {
                        m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::MOV,
                            m_currentConditionReg,
                            "-" + ownedCondReg);
                    } else {
                        m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::CMP,
                            m_currentConditionReg,
                            previousConditionReg,
                            previousConditionReg,   // if outer condition is false, propagate false
                            "-" + ownedCondReg      // if outer condition is true, flip the new condition
                            );
                    }
                    ifStatementNode->getElseStatement()->visit(*this);
                }

                // restore previous
                m_currentConditionReg = std::move(previousConditionReg);
                m_ifScopeCount--;
                assert(m_ifScopeCount >= 0);
            }

            virtual void nodeVisit(AST::DeclarationNode *declarationNode) {
                if(declarationNode->isOrdinaryType() && !declarationNode->isConst()) {
                    AST::ExpressionNode *initExpr = declarationNode->getExpression();
                    if(initExpr) {
                        std::string lhsReg = m_declaredSymbolRegisterTable.getRegisterName(declarationNode);
                        if(m_ifScopeCount == 0) {
                            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::MOV,
                                lhsReg,
                                ExpressionReducer::reduce(m_declaredSymbolRegisterTable, m_assemblyDB, initExpr));
                        } else {
                            m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::CMP, 
                                lhsReg,
                                m_currentConditionReg,
                                lhsReg, // if condition is false, no change
                                ExpressionReducer::reduce(m_declaredSymbolRegisterTable, m_assemblyDB, initExpr));
                        }
                    }
                }
            }

            virtual void nodeVisit(AST::AssignmentNode *assignmentNode) {
                AST::VariableNode *lhsVar = assignmentNode->getVariable();
                AST::ExpressionNode *rhsExpr = assignmentNode->getExpression();

                std::string lhsReg = ExpressionReducer::reduce(m_declaredSymbolRegisterTable, m_assemblyDB, lhsVar);
                std::string rhsReg = ExpressionReducer::reduce(m_declaredSymbolRegisterTable, m_assemblyDB, rhsExpr);

                if(m_ifScopeCount == 0) {
                    m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::MOV,
                        lhsReg,
                        rhsReg);
                } else {
                    m_assemblyDB.insertInstruction(ARBAssemblyDatabase::OPCode::CMP, 
                        lhsReg,
                        m_currentConditionReg,
                        lhsReg, // if condition is false, no change
                        rhsReg);
                }
            }
    };

    AssignmentVisitor visitor(declaredSymbolRegisterTable, assemblyDB);
    ast->visit(visitor);
} 


void DeclaredSymbolRegisterTable::sendToAssemblyDB(ARBAssemblyDatabase &assemblyDB) const {      
    for(const auto &p: m_regNameToDecl) {
        const AST::DeclarationNode *decl = p.second;
        if(decl->isOrdinaryType()) {
            if(decl->isConst()) {
                AST::ExpressionNode *initExpr = (decl->getInitValue()) ? decl->getInitValue(): decl->getExpression();
                assemblyDB.declareUserParamRegister(p.first, ConstQualifiedExpressionReducer::reduceToValue(*this, initExpr));
            } else {
                assemblyDB.declareUserTempRegister(p.first);
            }
        }
    }
}

} /* END NAMESPACE */


int genCode(node *ast) {
    COGEN::ARBAssemblyDatabase assemblyDB;
    COGEN::DeclaredSymbolRegisterTable declaredSymbolRegisterTable =
        COGEN::createDeclaredSymbolRegisterTable(static_cast<AST::ASTNode *>(ast));

    declaredSymbolRegisterTable.sendToAssemblyDB(assemblyDB);
    COGEN::sendInstructionToAssemblyDB(assemblyDB, declaredSymbolRegisterTable, ast);

    printf("\n");
    printf("ARB Assembly Database\n");
    assemblyDB.print();
    
    return 0;
}