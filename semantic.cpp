#include "semantic.h"

#include "ast.h"
#include "symbol.h"

#include "common.h"
#include "parser.tab.h"

#include <algorithm>
#include <vector>
#include <unordered_map>
#include <list>
#include <sstream>

#include <cassert>

namespace SEMA{ /* START NAMESPACE */

class SourceLocation {
    private:
        std::vector<std::string> m_sourceFile;

    public:
        SourceLocation(FILE *f);
    
    public:
        const std::vector<std::string> &getLines() const { return m_sourceFile; }
        const std::string &getLine(int line) const { return m_sourceFile.at(line - 1); }
};

SourceLocation::SourceLocation(FILE *f) {
    assert(f != nullptr);

    char *line = NULL;
    size_t len = 0;

    rewind(f);
    while(getline(&line, &len, f) != -1) {
        std::string str(line);
        str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
        m_sourceFile.push_back(std::move(str));
    }

    if(line) {
        free(line);
    }
}

class SemanticAnalyzer {
    public:
        enum class EventType {
            Warning,
            Error,
            Unknown
        };

    public: /* Forward declarations for friend declaration */
        void setTempEventASTNode(const AST::ASTNode *astNode);
        void setTempEventEventType(EventType eventType);
    
    public:
        class Event {
            private:
                const AST::ASTNode *m_astNode = nullptr;
                EventType m_eventType = EventType::Unknown;
                
                AST::SourceLocation m_eventLoc;
                std::string m_message;

                bool m_useRef = false;
                AST::SourceLocation m_refLoc;
                std::string m_refMessage;

            public:
                Event(const AST::ASTNode *astNode, EventType eventType):
                    m_astNode(astNode), m_eventType(eventType) {}
            
            public:
                friend void SemanticAnalyzer::setTempEventASTNode(const AST::ASTNode *astNode);
                friend void SemanticAnalyzer::setTempEventEventType(EventType eventType);

            public:
                const AST::ASTNode *getASTNode() const { return m_astNode; }
                EventType getEventType() const { return m_eventType; }

                std::string &Message() { return m_message; }
                const std::string &Message()const { return m_message; }
                AST::SourceLocation &EventLoc() { return m_eventLoc; }
                const AST::SourceLocation &EventLoc()const { return m_eventLoc; }
                
                bool isUsingReference() const { return m_useRef; }
                void setUsingReference(bool useRef) { m_useRef = useRef; }
                std::string &RefMessage() { return m_refMessage; }
                const std::string &RefMessage() const { return m_refMessage; }
                AST::SourceLocation &RefLoc() { return m_refLoc; }
                const AST::SourceLocation &RefLoc() const { return m_refLoc; }
        };

        using EventID = size_t;

    private:
        std::vector<std::unique_ptr<Event>> m_eventList;
        std::unordered_map<const AST::ASTNode *, std::vector<EventID>> m_astEventLU;

        std::vector<EventID> m_errorEventList;
        std::vector<EventID> m_warningEventList;

    private:
        std::unique_ptr<Event> m_tempEvent = nullptr;
        bool m_tempEventValid = false;
    
    public:
        void resetAnalyzer() {
            m_eventList.clear();
            m_astEventLU.clear();
            m_errorEventList.clear();
            m_warningEventList.clear();
            m_tempEvent.reset();
            m_tempEventValid = false;
        }
    
    public:
        int getNumberEvents() const { return m_eventList.size(); }
        int getNumberErrors() const { return m_errorEventList.size(); }
        int getNumberWarnings() const { return m_warningEventList.size(); }

    public:
        EventID createEvent(const AST::ASTNode *astNode, EventType eventType);

        Event &getEvent(EventID eventID);
        const Event &getEventC(EventID eventID) const;
        const Event &getEvent(EventID eventID) const;

    public:
        void printEvent(EventID eventID, const SourceLocation &sourceLocation) const;

    public:
        void createTempEvent(const AST::ASTNode *astNode = nullptr, EventType eventType = EventType::Unknown);
        void dropTempEvent();
        EventID promoteTempEvent();
        Event &getTempEvent();
        const Event &getTempEventC() const;
        const Event &getTempEvent() const;

        bool isTempEventCreated() const { return m_tempEventValid; }
};

SemanticAnalyzer::EventID SemanticAnalyzer::createEvent(const AST::ASTNode *astNode, EventType eventType) {
    assert(astNode != nullptr);
    assert(eventType != EventType::Unknown);

    m_eventList.emplace_back(new Event(astNode, eventType));
    EventID id = m_eventList.size() - 1;
    m_astEventLU[astNode].push_back(id);
    if(eventType == EventType::Error) {
        m_errorEventList.push_back(id);
    } else {
        m_warningEventList.push_back(id);
    }
    
    return id;
}

SemanticAnalyzer::Event &SemanticAnalyzer::getEvent(EventID eventID) {
    return *(m_eventList.at(eventID));
}

const SemanticAnalyzer::Event &SemanticAnalyzer::getEventC(EventID eventID) const {
    return *(m_eventList.at(eventID));
}

const SemanticAnalyzer::Event &SemanticAnalyzer::getEvent(EventID eventID) const {
    return *(m_eventList.at(eventID));
}

void SemanticAnalyzer::printEvent(EventID eventID, const SourceLocation &sourceLocation) const {
    static const std::string tenSpace(10, ' ');
    static const std::string sevenSpace(7, ' ');

    const Event &event = getEvent(eventID);
    const AST::SourceLocation &eventLoc = event.EventLoc();
    
    printf("--------------------------------------------------------------------------\n");

    // Print event message
    std::string header;
    if(event.getEventType() == EventType::Error) {
        header = "Error";
    } else {
        header = "Warning";
    }
    printf("\033[1;31m%s-%d\033[0m", header.c_str(), eventID);
    printf("\033[1;39m: %s\033[0m\n", event.Message().c_str());

    // Print event location info
    if(AST::SourceLocation() != eventLoc) {
        if(eventLoc.firstLine == eventLoc.lastLine) {
            const std::string &line = sourceLocation.getLine(eventLoc.firstLine);
            printf("\033[0;32m%7d:%s\033[0m", eventLoc.firstLine, tenSpace.c_str());
            printf("\033[1;37m%s\033[0m\n", line.c_str());
            printf("%s %s", sevenSpace.c_str(), tenSpace.c_str());
            for(int colNumber = 0; colNumber < line.length(); colNumber++) {
                if(colNumber >= eventLoc.firstColumn - 1 && colNumber < eventLoc.lastColumn - 1) {
                    printf("\033[1;31m^\033[0m");
                } else {
                    printf(" ");
                }
            }
        } else {
            for(int lineNumber = eventLoc.firstLine; lineNumber <= eventLoc.lastLine; lineNumber++) {
                const std::string &line = sourceLocation.getLine(lineNumber);
                printf("\033[0;32m%7d:%s\033[0m", lineNumber, tenSpace.c_str());
                printf("\033[1;37m%s\033[0m\n", line.c_str());
            }
        }
        printf("\n");
    }

    // Print reference location info and reference message
    if(event.isUsingReference()) {
        const AST::SourceLocation &refLoc = event.RefLoc();

        printf("\n");

        // Print reference message
        printf("\033[1;37mInfo\033[0m");
        printf("\033[1;39m: %s\033[0m\n", event.RefMessage().c_str());

        // Print reference location info
        if(AST::SourceLocation() != refLoc) {
            if(refLoc.firstLine == refLoc.lastLine) {
                const std::string &line = sourceLocation.getLine(refLoc.firstLine);
                printf("\033[0;32m%7d:%s\033[0m", refLoc.firstLine, tenSpace.c_str());
                printf("\033[1;37m%s\033[0m\n", line.c_str());
                printf("%s %s", sevenSpace.c_str(), tenSpace.c_str());
                for(int colNumber = 0; colNumber < line.length(); colNumber++) {
                    if(colNumber >= refLoc.firstColumn - 1 && colNumber < refLoc.lastColumn - 1) {
                        printf("\033[1;33m~\033[0m");
                    } else {
                        printf(" ");
                    }
                }
            } else {
                for(int lineNumber = refLoc.firstLine; lineNumber <= refLoc.lastLine; lineNumber++) {
                    const std::string &line = sourceLocation.getLine(lineNumber);
                    printf("\033[0;32m%7d:%s\033[0m", lineNumber, tenSpace.c_str());
                    printf("\033[1;37m%s\033[0m\n", line.c_str());
                }
            }
            printf("\n");
        }
    }
}

void SemanticAnalyzer::createTempEvent(const AST::ASTNode *astNode, EventType eventType) {
    assert(m_tempEventValid == false);
    assert(m_tempEvent == nullptr);
    m_tempEventValid = true;
    m_tempEvent.reset(new Event(astNode, eventType));
}

void SemanticAnalyzer::setTempEventASTNode(const AST::ASTNode *astNode) {
    assert(m_tempEventValid == true);
    assert(m_tempEvent != nullptr);
    m_tempEvent->m_astNode = astNode;
}

void SemanticAnalyzer::setTempEventEventType(EventType eventType) {
    assert(m_tempEventValid == true);
    assert(m_tempEvent != nullptr);
    m_tempEvent->m_eventType = eventType;
}

void SemanticAnalyzer::dropTempEvent() {
    assert(m_tempEventValid == true);
    assert(m_tempEvent != nullptr);
    m_tempEventValid = false;
    m_tempEvent.reset(nullptr);
}

SemanticAnalyzer::EventID SemanticAnalyzer::promoteTempEvent() {
    assert(m_tempEventValid == true);
    assert(m_tempEvent != nullptr);

    EventType eventType = m_tempEvent->getEventType();
    const AST::ASTNode *astNode = m_tempEvent->getASTNode();
    assert(eventType != EventType::Unknown);
    assert(astNode != nullptr);

    m_tempEventValid = false;
    m_eventList.push_back(std::move(m_tempEvent));
    assert(m_tempEvent == nullptr);

    EventID id = m_eventList.size() - 1;

    m_astEventLU[astNode].push_back(id);
    if(eventType == EventType::Error) {
        m_errorEventList.push_back(id);
    } else {
        m_warningEventList.push_back(id);
    }

    return id;
}

SemanticAnalyzer::Event &SemanticAnalyzer::getTempEvent() {
    assert(m_tempEventValid == true);
    assert(m_tempEvent != nullptr);
    return *m_tempEvent;
}

const SemanticAnalyzer::Event &SemanticAnalyzer::getTempEventC() const {
    assert(m_tempEventValid == true);
    assert(m_tempEvent != nullptr);
    return *m_tempEvent;
}

const SemanticAnalyzer::Event &SemanticAnalyzer::getTempEvent() const {
    assert(m_tempEventValid == true);
    assert(m_tempEvent != nullptr);
    return *m_tempEvent;
}

class PredefinedVariableCreater: public AST::Visitor {
    private:
        virtual void preNodeVisit(AST::ScopeNode *scopeNode) {
            AST::DeclarationsNode *decls = scopeNode->getDeclarations();

            // Manually add predefined variables onto the AST tree
            /*
                result vec4 gl_FragColor ;
                result bool gl_FragDepth ;
                result vec4 gl_FragCoord ;
                attribute vec4 gl_TexCoord ;
                attribute vec4 gl_Color;
                attribute vec4 gl_Secondary ;
                attribute vec4 gl_FogFragCoord ;
                uniform vec4 gl_Light_Half ;
                uniform vec4 gl_Light_Ambient ;
                uniform vec4 gl_Material_Shininess ;
                uniform vec4 env1;
                uniform vec4 env2;
                uniform vec4 env3;
            */

            // result vec4 gl_FragColor ;
            AST::DeclarationNode *node_gl_FragColor = new AST::DeclarationNode("gl_FragColor", false, VEC4_T);
            node_gl_FragColor->setResultType();

            // result bool gl_FragDepth ;
            AST::DeclarationNode *node_gl_FragDepth = new AST::DeclarationNode("gl_FragDepth", false, BOOL_T);
            node_gl_FragDepth->setResultType();

            // result vec4 gl_FragCoord ;
            AST::DeclarationNode *node_gl_FragCoord = new AST::DeclarationNode("gl_FragCoord", false, VEC4_T);
            node_gl_FragCoord->setResultType();

            // attribute vec4 gl_TexCoord ;
            AST::DeclarationNode *node_gl_TexCoord = new AST::DeclarationNode("gl_TexCoord", false, VEC4_T);
            node_gl_TexCoord->setAttributeType();

            // attribute vec4 gl_Color;
            AST::DeclarationNode *node_gl_Color = new AST::DeclarationNode("gl_Color", false, VEC4_T);
            node_gl_Color->setAttributeType();

            // attribute vec4 gl_Secondary ;
            AST::DeclarationNode *node_gl_Secondary = new AST::DeclarationNode("gl_Secondary", false, VEC4_T);
            node_gl_Secondary->setAttributeType();

            // attribute vec4 gl_FogFragCoord ;
            AST::DeclarationNode *node_gl_FogFragCoord = new AST::DeclarationNode("gl_FogFragCoord", false, VEC4_T);
            node_gl_FogFragCoord->setAttributeType();

            // uniform vec4 gl_Light_Half ;
            AST::DeclarationNode *node_gl_Light_Half = new AST::DeclarationNode("gl_Light_Half", false, VEC4_T);
            node_gl_Light_Half->setUniformType();

            // uniform vec4 gl_Light_Ambient ;
            AST::DeclarationNode *node_gl_Light_Ambient = new AST::DeclarationNode("gl_Light_Ambient", false, VEC4_T);
            node_gl_Light_Ambient->setUniformType();

            // uniform vec4 gl_Material_Shininess ;
            AST::DeclarationNode *node_gl_Material_Shininess = new AST::DeclarationNode("gl_Material_Shininess", false, VEC4_T);
            node_gl_Material_Shininess->setUniformType();

            // uniform vec4 env1;
            AST::DeclarationNode *node_env1 = new AST::DeclarationNode("env1", false, VEC4_T);
            node_env1->setUniformType();

            // uniform vec4 env2;
            AST::DeclarationNode *node_env2 = new AST::DeclarationNode("env2", false, VEC4_T);
            node_env2->setUniformType();

            // uniform vec4 env3;
            AST::DeclarationNode *node_env3 = new AST::DeclarationNode("env3", false, VEC4_T);
            node_env3->setUniformType();

            // Insert into AST
            decls->pushFrontDeclaration(node_env3);
            decls->pushFrontDeclaration(node_env2);
            decls->pushFrontDeclaration(node_env1);
            decls->pushFrontDeclaration(node_gl_Material_Shininess);
            decls->pushFrontDeclaration(node_gl_Light_Ambient);
            decls->pushFrontDeclaration(node_gl_Light_Half);
            decls->pushFrontDeclaration(node_gl_FogFragCoord);
            decls->pushFrontDeclaration(node_gl_Secondary);
            decls->pushFrontDeclaration(node_gl_Color);
            decls->pushFrontDeclaration(node_gl_TexCoord);
            decls->pushFrontDeclaration(node_gl_FragCoord);
            decls->pushFrontDeclaration(node_gl_FragDepth);
            decls->pushFrontDeclaration(node_gl_FragColor);
        }
};

class SymbolDeclVisitor: public AST::Visitor {
    private:
        ST::SymbolTable &m_symbolTable;
        SEMA::SemanticAnalyzer &m_semaAnalyzer;
    public:
        SymbolDeclVisitor(ST::SymbolTable &symbolTable, SEMA::SemanticAnalyzer &semaAnalyzer):
            m_symbolTable(symbolTable), m_semaAnalyzer(semaAnalyzer) {}

    private:
        virtual void preNodeVisit(AST::IdentifierNode *identifierNode) {
            m_symbolTable.markSymbolRefPos(identifierNode);
        }

        virtual void preNodeVisit(AST::DeclarationNode *declarationNode) {
            // Declare the symbol before traversing the possible evaluation on rhs
            AST::DeclarationNode *redecl = m_symbolTable.declareSymbol(declarationNode);

            if(redecl != nullptr) {
                std::stringstream ss;
                ss << "Duplicate declaration of '" << (declarationNode->isConst()? "const " : "") << AST::getTypeString(declarationNode->getType()) <<
                    " " << declarationNode->getName() << "' at " << AST::getSourceLocationString(declarationNode->getSourceLocation()) << ". " <<
                    "Previously declared at " << AST::getSourceLocationString(redecl->getSourceLocation()) << ".";

                auto id = m_semaAnalyzer.createEvent(declarationNode, SemanticAnalyzer::EventType::Error);
                m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
                m_semaAnalyzer.getEvent(id).EventLoc() = declarationNode->getSourceLocation();

                m_semaAnalyzer.getEvent(id).setUsingReference(true);
                if(redecl->isOrdinaryType()) {
                    m_semaAnalyzer.getEvent(id).RefMessage() = "Previously declared here:";
                    m_semaAnalyzer.getEvent(id).RefLoc() = redecl->getSourceLocation();
                } else if (redecl->isAttributeType()){
                    m_semaAnalyzer.getEvent(id).RefMessage() = "Predefined Variable: 'attribute " +
                        AST::getTypeString(redecl->getType()) + " " + redecl->getName() + "'.";
                } else if (redecl->isUniformType()){
                    m_semaAnalyzer.getEvent(id).RefMessage() = "Predefined Variable: 'uniform " +
                        AST::getTypeString(redecl->getType()) + " " + redecl->getName() + "'.";
                } else if (redecl->isResultType()){
                    m_semaAnalyzer.getEvent(id).RefMessage() = "Predefined Variable: 'result " +
                        AST::getTypeString(redecl->getType()) + " " + redecl->getName() + "'.";
                } else {
                    assert(0);
                }

                return;
            }
        }

        virtual void preNodeVisit(AST::NestedScopeNode *nestedScopeNode) {
            m_symbolTable.enterScope();
        }
        virtual void postNodeVisit(AST::NestedScopeNode *nestedScopeNode) {
            m_symbolTable.exitScope();
        }

        virtual void preNodeVisit(AST::ScopeNode *scopeNode) {
            m_symbolTable.enterScope();
        }
        virtual void postNodeVisit(AST::ScopeNode *scopeNode) {
            m_symbolTable.exitScope();
        }
};

class WriteOnlyFinder: public AST::Visitor {
    private:
        std::vector<const AST::VariableNode *> m_writeOnlyVars;
    private:
        virtual void postNodeVisit(AST::IdentifierNode *identifierNode) {
            if(identifierNode->isWriteOnly()) {
                m_writeOnlyVars.push_back(identifierNode);
            }
        }
        virtual void postNodeVisit(AST::IndexingNode *indexingNode) {
            if(indexingNode->isWriteOnly()) {
                m_writeOnlyVars.push_back(indexingNode);
            }
        }
    private:
        virtual void nodeVisit(AST::IndexingNode *indexingNode) {
            /* Do not traverse into IndexingNode */
        }
    public:
        const std::vector<const AST::VariableNode *> &getWriteOnlyVars() const { return m_writeOnlyVars; }
};

enum class DataTypeCategory {
    Boolean,
    Arithmetic
};

DataTypeCategory getDataTypeCategory(int dataType) {
    switch(dataType) {
        case BOOL_T:
        case BVEC2_T:
        case BVEC3_T:
        case BVEC4_T:
            return DataTypeCategory::Boolean;
        case INT_T:
        case IVEC2_T:
        case IVEC3_T:
        case IVEC4_T:
        case FLOAT_T:
        case VEC2_T:
        case VEC3_T:
        case VEC4_T:
            return DataTypeCategory::Arithmetic;
        default:
            assert(0);
    }
}

int getDataTypeOrder(int dataType) {
    switch(dataType) {
        case BOOL_T:
        case INT_T:
        case FLOAT_T:
            return 1;
        case BVEC2_T:
        case IVEC2_T:
        case VEC2_T:
            return 2;
        case BVEC3_T:
        case IVEC3_T:
        case VEC3_T:
            return 3;
        case BVEC4_T:
        case IVEC4_T:
        case VEC4_T:
            return 4;
        default:
            assert(0);
    }
}

int getDataTypeBaseType(int dataType) {
    switch(dataType) {
        case BOOL_T:
        case BVEC2_T:
        case BVEC3_T:
        case BVEC4_T:
            return BOOL_T;
        case INT_T:
        case IVEC2_T:
        case IVEC3_T:
        case IVEC4_T:
            return INT_T;
        case FLOAT_T:
        case VEC2_T:
        case VEC3_T:
        case VEC4_T:
            return FLOAT_T;
        default:
            assert(0);
    }
}

class TypeChecker: public AST::Visitor {
    private:
        ST::SymbolTable &m_symbolTable;
        SEMA::SemanticAnalyzer &m_semaAnalyzer;
    public:
        TypeChecker(ST::SymbolTable &symbolTable, SEMA::SemanticAnalyzer &semaAnalyzer):
            m_symbolTable(symbolTable), m_semaAnalyzer(semaAnalyzer) {}
    
    private:
        virtual void preNodeVisit(AST::IdentifierNode *identifierNode);

    private:
        virtual void postNodeVisit(AST::UnaryExpressionNode *unaryExpressionNode);
        virtual void postNodeVisit(AST::BinaryExpressionNode *binaryExpressionNode);
        virtual void postNodeVisit(AST::IndexingNode *indexingNode);
        virtual void postNodeVisit(AST::FunctionNode *functionNode);
        virtual void postNodeVisit(AST::ConstructorNode *constructorNode);
        virtual void postNodeVisit(AST::DeclarationNode *declarationNode);
        virtual void postNodeVisit(AST::IfStatementNode *ifStatementNode);
        virtual void postNodeVisit(AST::AssignmentNode *assignmentNode);

    private:
        int inferDataType(int op, int rhsDataType);
        int inferDataType(int op, int lhsDataType, int rhsDataType);
};

void TypeChecker::preNodeVisit(AST::IdentifierNode *identifierNode) {
    AST::DeclarationNode *decl = m_symbolTable.getSymbolDecl(identifierNode);

    if(decl == nullptr) {
        // Update info in identifierNode
        identifierNode->setExpressionType(ANY_TYPE);
        identifierNode->setDeclaration(nullptr);

        std::stringstream ss;
        ss << "Missing declaration for symbol'" << identifierNode->getName() <<
        "' at " << AST::getSourceLocationString(identifierNode->getSourceLocation()) << ".";

        auto id = m_semaAnalyzer.createEvent(identifierNode, SemanticAnalyzer::EventType::Error);
        m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
        m_semaAnalyzer.getEvent(id).EventLoc() = identifierNode->getSourceLocation();

        return;
    }

    assert(decl->getName() == identifierNode->getName());

    // Update info in identifierNode
    identifierNode->setExpressionType(decl->getType());
    identifierNode->setDeclaration(decl);
}

void TypeChecker::postNodeVisit(AST::UnaryExpressionNode *unaryExpressionNode) {
    const AST::ExpressionNode *rhsExpr = unaryExpressionNode->getExpression();
    int rhsDataType = rhsExpr->getExpressionType();
    bool rhsIsConst = rhsExpr->isConst();
    int op = unaryExpressionNode->getOperator();

    int resultDataType = ANY_TYPE;
    if(rhsDataType != ANY_TYPE) {
        bool isLegal = true;

        // Firstly, check for Write-Only
        WriteOnlyFinder writeOnlyFinder;
        unaryExpressionNode->visit(writeOnlyFinder);
        const std::vector<const AST::VariableNode *> &writeOnlyVars = writeOnlyFinder.getWriteOnlyVars();
        if(!writeOnlyVars.empty()) {
            isLegal = false;

            std::stringstream ss;
            ss << "Operand in unary expression at " << AST::getSourceLocationString(unaryExpressionNode->getSourceLocation()) <<
                " has write-only Result type at " << AST::getSourceLocationString(rhsExpr->getSourceLocation()) << ".";

            auto id = m_semaAnalyzer.createEvent(unaryExpressionNode, SemanticAnalyzer::EventType::Error);
            m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
            m_semaAnalyzer.getEvent(id).EventLoc() = unaryExpressionNode->getSourceLocation();

            m_semaAnalyzer.getEvent(id).setUsingReference(true);
            m_semaAnalyzer.getEvent(id).RefMessage() = "The first write-only Result variable is '" + writeOnlyVars.front()->getName() + "':";
            m_semaAnalyzer.getEvent(id).RefLoc() = writeOnlyVars.front()->getSourceLocation();
        }

        // Secondly, Type check
        m_semaAnalyzer.createTempEvent(unaryExpressionNode, SemanticAnalyzer::EventType::Error);

        resultDataType = inferDataType(op, rhsDataType);
        
        if(resultDataType == ANY_TYPE) {
            isLegal = false;

            std::stringstream ss;
            ss << "Operand in unary expression at " << AST::getSourceLocationString(unaryExpressionNode->getSourceLocation()) <<
                " has non-compatible type at " << AST::getSourceLocationString(rhsExpr->getSourceLocation()) << ".";

            m_semaAnalyzer.getTempEvent().Message() = std::move(ss.str());
            m_semaAnalyzer.getTempEvent().EventLoc() = unaryExpressionNode->getSourceLocation();

            m_semaAnalyzer.promoteTempEvent();
        } else {
            m_semaAnalyzer.dropTempEvent();
        }

        if(!isLegal) {
            resultDataType = ANY_TYPE;
        }
    }
    
    unaryExpressionNode->setExpressionType(resultDataType);
    unaryExpressionNode->setConst(rhsIsConst);
}

void TypeChecker::postNodeVisit(AST::BinaryExpressionNode *binaryExpressionNode) {
    const AST::ExpressionNode *lhsExpr = binaryExpressionNode->getLeftExpression();
    const AST::ExpressionNode *rhsExpr = binaryExpressionNode->getRightExpression();
    
    int lhsDataType = lhsExpr->getExpressionType();
    int rhsDataType = rhsExpr->getExpressionType();

    bool lhsIsConst = lhsExpr->isConst();
    bool rhsIsConst = rhsExpr->isConst();

    int op = binaryExpressionNode->getOperator();

    int resultDataType = ANY_TYPE;
    if(lhsDataType != ANY_TYPE && rhsDataType != ANY_TYPE) {
        bool isLegal = true;

        // Firstly, check for Write-Only
        WriteOnlyFinder writeOnlyFinder;
        binaryExpressionNode->visit(writeOnlyFinder);
        const std::vector<const AST::VariableNode *> &writeOnlyVars = writeOnlyFinder.getWriteOnlyVars();
        if(!writeOnlyVars.empty()) {
            isLegal = false;

            std::stringstream ss;
            ss << "Operands in binary expression at " << AST::getSourceLocationString(binaryExpressionNode->getSourceLocation()) <<
                " has write-only Result type.";

            auto id = m_semaAnalyzer.createEvent(binaryExpressionNode, SemanticAnalyzer::EventType::Error);
            m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
            m_semaAnalyzer.getEvent(id).EventLoc() = binaryExpressionNode->getSourceLocation();

            m_semaAnalyzer.getEvent(id).setUsingReference(true);
            m_semaAnalyzer.getEvent(id).RefMessage() = "The first write-only Result variable is '" + writeOnlyVars.front()->getName() + "':";
            m_semaAnalyzer.getEvent(id).RefLoc() = writeOnlyVars.front()->getSourceLocation();
        }

        // Secondly, Type check
        m_semaAnalyzer.createTempEvent(binaryExpressionNode, SemanticAnalyzer::EventType::Error);

        resultDataType = inferDataType(op, lhsDataType, rhsDataType);

        if(resultDataType == ANY_TYPE) {
            isLegal = false;

            std::stringstream ss;
            ss << "Operands in binary expression at " << AST::getSourceLocationString(binaryExpressionNode->getSourceLocation()) <<
                " have non-compatible type.";

            m_semaAnalyzer.getTempEvent().Message() = std::move(ss.str());
            m_semaAnalyzer.getTempEvent().EventLoc() = binaryExpressionNode->getSourceLocation();

            m_semaAnalyzer.promoteTempEvent();
        } else {
            m_semaAnalyzer.dropTempEvent();
        }

        if(!isLegal) {
            resultDataType = ANY_TYPE;
        }
    }

    binaryExpressionNode->setExpressionType(resultDataType);
    binaryExpressionNode->setConst(lhsIsConst && rhsIsConst);
}

void TypeChecker::postNodeVisit(AST::IndexingNode *indexingNode) {
    /*
     * - The index into a vector (e.g. 1 in foo[1]) must be in the range [0, i1] if the vector
     * has type veci. For example, the maximum index into a variable of type vec2 is 1.
     * - The result type of indexing into a vector is the base type associated with that vector’s
     * type. For example, if v has type bvec2 then v[0] has type bool.
     */
    int resultDataType = ANY_TYPE;

    const AST::IdentifierNode *identifier = indexingNode->getIdentifier();            
    int identifierDataType = identifier->getExpressionType();
    int identifierIsConst = identifier->isConst();
    // Identifier must be a vector
    int identifierTypeOrder = getDataTypeOrder(identifierDataType);
    if(identifierTypeOrder > 1) {
        const AST::IntLiteralNode *index = 
            dynamic_cast<AST::IntLiteralNode *>(indexingNode->getIndexExpression());
        int indexVal = index->getVal();
        // Index must be valid
        if(indexVal >= 0 && indexVal < identifierTypeOrder) {
            // The type of this indexing node is the base type for identifier node
            int identifierBaseType = getDataTypeBaseType(identifierDataType);
            resultDataType = identifierBaseType;
        } else {
            std::stringstream ss;
            ss << "Invalid indexing of vector variable '" << identifier->getName() << 
                "' of type '" << (identifierIsConst ? "const " : "") << AST::getTypeString(identifierDataType) << "' at " <<
                AST::getSourceLocationString(indexingNode->getSourceLocation()) << ".";

            auto id = m_semaAnalyzer.createEvent(indexingNode, SemanticAnalyzer::EventType::Error);
            m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
            m_semaAnalyzer.getEvent(id).EventLoc() = indexingNode->getSourceLocation();

            m_semaAnalyzer.getEvent(id).setUsingReference(true);
            ss = std::stringstream();
            ss << "Expecting an index value between 0 to " << (identifierTypeOrder - 1) << ".";
            m_semaAnalyzer.getEvent(id).RefMessage() = std::move(ss.str());
        }
    } else {
        std::stringstream ss;
        ss << "Invalid indexing of non-vector variable '" << identifier->getName() << 
            "' of type '" << (identifierIsConst ? "const " : "") << AST::getTypeString(identifierDataType) << "' at " <<
            AST::getSourceLocationString(indexingNode->getSourceLocation()) << ".";

        auto id = m_semaAnalyzer.createEvent(indexingNode, SemanticAnalyzer::EventType::Error);
        m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
        m_semaAnalyzer.getEvent(id).EventLoc() = indexingNode->getSourceLocation();
    }

    indexingNode->setExpressionType(resultDataType);
}

void TypeChecker::postNodeVisit(AST::FunctionNode *functionNode) {
    int resultDataType = ANY_TYPE;

    bool legalFunctionCall = true;

    // Firstly, check for argument type
    const std::string &funcName = functionNode->getName();
    AST::ExpressionsNode *exprs = functionNode->getArgumentExpressions();
    const std::vector<AST::ExpressionNode *> &args = exprs->getExpressionList();
    if(funcName == "rsq") {
        bool isLegal = false;
        if(args.size() == 1) {
            const AST::ExpressionNode *arg1 = args.front();
            int arg1Type = arg1->getExpressionType();
            if(arg1Type == FLOAT_T || arg1Type == INT_T) {
                // float rsq(float);
                // float rsq(int);
                resultDataType = FLOAT_T;
                isLegal = true;
            }
        }

        if(!isLegal) {
            legalFunctionCall = false;
            
            std::stringstream ss;
            ss << "Unmatched function parameters when calling function 'rsq' at " <<
                AST::getSourceLocationString(functionNode->getSourceLocation()) << ".";

            auto id = m_semaAnalyzer.createEvent(functionNode, SemanticAnalyzer::EventType::Error);
            m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
            m_semaAnalyzer.getEvent(id).EventLoc() = functionNode->getSourceLocation();

            m_semaAnalyzer.getEvent(id).setUsingReference(true);
            m_semaAnalyzer.getEvent(id).RefMessage() = "Expecting function argument 'float' or 'int'.";
        }
    } else if (funcName == "dp3") {
        bool isLegal = false;
        if(args.size() == 2) {
            const AST::ExpressionNode *arg1 = args[0];
            int arg1Type = arg1->getExpressionType();
            const AST::ExpressionNode *arg2 = args[1];
            int arg2Type = arg2->getExpressionType();

            if(arg1Type == arg2Type) {
                if(arg1Type == VEC3_T || arg1Type == VEC4_T || arg1Type == IVEC3_T || arg1Type == IVEC4_T) {
                    // float dp3(vec4, vec4);
                    // float dp3(vec3, vec3);
                    // float dp3(ivec4, ivec4);
                    // float dp3(ivec3, ivec3);
                    resultDataType = FLOAT_T;
                    isLegal = true;
                }
            }
        }

        if(!isLegal) {
            legalFunctionCall = false;

            std::stringstream ss;
            ss << "Unmatched function parameters when calling function 'dp3' at " <<
                AST::getSourceLocationString(functionNode->getSourceLocation()) << ".";

            auto id = m_semaAnalyzer.createEvent(functionNode, SemanticAnalyzer::EventType::Error);
            m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
            m_semaAnalyzer.getEvent(id).EventLoc() = functionNode->getSourceLocation();

            m_semaAnalyzer.getEvent(id).setUsingReference(true);
            m_semaAnalyzer.getEvent(id).RefMessage() =
                "Expecting function arguments 'vec4, vec4' or 'vec3, vec3' or 'ivec4, ivec4' or 'ivec3, ivec3'.";
        }
    } else if (funcName == "lit") {
        bool isLegal = false;
        if(args.size() == 1) {
            const AST::ExpressionNode *arg1 = args.front();
            int arg1Type = arg1->getExpressionType();
            if(arg1Type == VEC4_T) {
                // vec4 lit(vec4);
                resultDataType = VEC4_T;
                isLegal = true;
            }
        }

        if(!isLegal) {
            legalFunctionCall = false;

            std::stringstream ss;
            ss << "Unmatched function parameters when calling function 'lit' at " <<
                AST::getSourceLocationString(functionNode->getSourceLocation()) << ".";

            auto id = m_semaAnalyzer.createEvent(functionNode, SemanticAnalyzer::EventType::Error);
            m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
            m_semaAnalyzer.getEvent(id).EventLoc() = functionNode->getSourceLocation();

            m_semaAnalyzer.getEvent(id).setUsingReference(true);
            m_semaAnalyzer.getEvent(id).RefMessage() = "Expecting function argument 'vec4'.";
        }
    } else {
        assert(0);
    }

    // Secondly, check for Write-Only
    if(legalFunctionCall) {
        WriteOnlyFinder writeOnlyFinder;
        functionNode->visit(writeOnlyFinder);
        const std::vector<const AST::VariableNode *> &writeOnlyVars = writeOnlyFinder.getWriteOnlyVars();
        if(!writeOnlyVars.empty()) {
            resultDataType = ANY_TYPE;

            std::stringstream ss;
            ss << "Arguments in function call at " << AST::getSourceLocationString(functionNode->getSourceLocation()) <<
                " has write-only Result type.";

            auto id = m_semaAnalyzer.createEvent(functionNode, SemanticAnalyzer::EventType::Error);
            m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
            m_semaAnalyzer.getEvent(id).EventLoc() = functionNode->getSourceLocation();

            m_semaAnalyzer.getEvent(id).setUsingReference(true);
            m_semaAnalyzer.getEvent(id).RefMessage() = "The first write-only Result variable is '" + writeOnlyVars.front()->getName() + "':";
            m_semaAnalyzer.getEvent(id).RefLoc() = writeOnlyVars.front()->getSourceLocation();
        }
    }

    functionNode->setExpressionType(resultDataType);
}

void TypeChecker::postNodeVisit(AST::ConstructorNode *constructorNode) {
    /*
    * Constructors for basic types (bool, int, float) must have one argument that exactly
    * matches that type. Constructors for vector types must have as many arguments as there
    * are elements in the vector, and the types of each argument must be exactly the type of the
    * basic type corresponding to the vector type. For example, bvec2(true,false) is valid ,
    * whereas bvec2(1,true) is invalid.
    */

    int resultDataType = ANY_TYPE;

    // Firstly, check for argument type
    bool resultIsConst = false;

    int constructorType = constructorNode->getConstructorType();
    int constructorTypeBase = getDataTypeBaseType(constructorType);
    int constructorTypeOrder = getDataTypeOrder(constructorType);
    AST::ExpressionsNode *exprs = constructorNode->getArgumentExpressions();
    const std::vector<AST::ExpressionNode *> &args = exprs->getExpressionList();

    bool argLegal = false;
    if(constructorTypeOrder == args.size()) {
        resultIsConst = true;
        argLegal = true;
        for(const AST::ExpressionNode *arg : args) {
            if(arg->getExpressionType() != constructorTypeBase) {
                argLegal = false;
            }
            resultIsConst &= arg->isConst();
        }

        if(argLegal) {
            resultDataType = constructorType;
        }
    }

    if(!argLegal) {
        std::stringstream ss;
        ss << "Unmatched constructor parameters when calling constructor for '" << AST::getTypeString(constructorType) << "' at " <<
            AST::getSourceLocationString(constructorNode->getSourceLocation()) << ".";

        auto id = m_semaAnalyzer.createEvent(constructorNode, SemanticAnalyzer::EventType::Error);
        m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
        m_semaAnalyzer.getEvent(id).EventLoc() = constructorNode->getSourceLocation();

        m_semaAnalyzer.getEvent(id).setUsingReference(true);
        ss = std::stringstream();
        ss << "Expecting constructor argument" << (constructorTypeOrder > 1 ? "s" : "") << " '";
        std::string ctorTypeBaseStr = AST::getTypeString(constructorTypeBase);
        ss << ctorTypeBaseStr;
        for(int i = 1; i < constructorTypeOrder; i++) {
            ss << ", " << ctorTypeBaseStr;
        }
        ss << "'.";
        m_semaAnalyzer.getEvent(id).RefMessage() = std::move(ss.str());
    } else {
        // Secondly, check for Write-Only
        WriteOnlyFinder writeOnlyFinder;
        constructorNode->visit(writeOnlyFinder);
        const std::vector<const AST::VariableNode *> &writeOnlyVars = writeOnlyFinder.getWriteOnlyVars();
        if(!writeOnlyVars.empty()) {
            resultDataType = ANY_TYPE;

            std::stringstream ss;
            ss << "Arguments in constructor call at " << AST::getSourceLocationString(constructorNode->getSourceLocation()) <<
                " has write-only Result type.";

            auto id = m_semaAnalyzer.createEvent(constructorNode, SemanticAnalyzer::EventType::Error);
            m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
            m_semaAnalyzer.getEvent(id).EventLoc() = constructorNode->getSourceLocation();

            m_semaAnalyzer.getEvent(id).setUsingReference(true);
            m_semaAnalyzer.getEvent(id).RefMessage() = "The first write-only Result variable is '" + writeOnlyVars.front()->getName() + "':";
            m_semaAnalyzer.getEvent(id).RefLoc() = writeOnlyVars.front()->getSourceLocation();
        }
    }

    constructorNode->setExpressionType(resultDataType);
    constructorNode->setConst(resultIsConst);
}

void TypeChecker::postNodeVisit(AST::DeclarationNode *declarationNode) {
    /* Don't check predefined variables */
    if(!declarationNode->isOrdinaryType()) {
        return;
    }

    /*
     * Initialization
     * 
     * - const qualified variables must be initialized with a literal value or with a uniform
     * variable, not an expression.
     * - The value assigned to a variable (this includes variables declared with an initial value)
     * must have the same type as that variable, or a type that can be widened to the correct type.
     */
    const AST::ExpressionNode *initExpr = declarationNode->getExpression();
    if(declarationNode->isConst()) {
        if(initExpr == nullptr) {
            std::stringstream ss;
            ss << "Const qualified variable 'const " << AST::getTypeString(declarationNode->getType()) << " " << declarationNode->getName() <<
                "' is missing initialization at " << AST::getSourceLocationString(declarationNode->getSourceLocation()) << ".";

            auto id = m_semaAnalyzer.createEvent(declarationNode, SemanticAnalyzer::EventType::Error);
            m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
            m_semaAnalyzer.getEvent(id).EventLoc() = declarationNode->getSourceLocation();

        } else if (!initExpr->isConst()) {
            std::stringstream ss;
            ss << "Const qualified variable 'const " << AST::getTypeString(declarationNode->getType()) << " " << declarationNode->getName() <<
                "' is initialized to a non-const qualified expression at " << AST::getSourceLocationString(initExpr->getSourceLocation()) << ".";

            auto id = m_semaAnalyzer.createEvent(declarationNode, SemanticAnalyzer::EventType::Error);
            m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
            m_semaAnalyzer.getEvent(id).EventLoc() = declarationNode->getSourceLocation();
        }
    }

    if(initExpr != nullptr) {
        int rhsDataType = initExpr->getExpressionType();
        int lhsDataType = declarationNode->getType();

        if(rhsDataType == ANY_TYPE) {
            std::stringstream ss;
            ss << "Variable declaration of '" << (declarationNode->isConst() ? "const " : "") << AST::getTypeString(lhsDataType) << " " << declarationNode->getName() <<
                "' at " << AST::getSourceLocationString(declarationNode->getSourceLocation())  << ", is initialized to an unknown type at " <<
                AST::getSourceLocationString(initExpr->getSourceLocation()) << " due to previous error(s).";
            
            auto id = m_semaAnalyzer.createEvent(declarationNode, SemanticAnalyzer::EventType::Error);
            m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
            m_semaAnalyzer.getEvent(id).EventLoc() = declarationNode->getSourceLocation();
        } else if(lhsDataType != rhsDataType) {
            std::stringstream ss;
            ss << "Variable declaration of '" << (declarationNode->isConst() ? "const " : "") << AST::getTypeString(lhsDataType) << " " << declarationNode->getName() <<
                "' at " << AST::getSourceLocationString(declarationNode->getSourceLocation())  << ", is initialized to a noncompatible type '"<< AST::getTypeString(rhsDataType) <<
                "' at " << AST::getSourceLocationString(initExpr->getSourceLocation()) << ".";

            auto id = m_semaAnalyzer.createEvent(declarationNode, SemanticAnalyzer::EventType::Error);
            m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
            m_semaAnalyzer.getEvent(id).EventLoc() = declarationNode->getSourceLocation();
        }
    }
}

void TypeChecker::postNodeVisit(AST::IfStatementNode *ifStatementNode) {
    /*
     * The expression that determines which branch of an if statement should be taken must
     * have the type bool (not bvec).
     */
    const AST::ExpressionNode *cond = ifStatementNode->getConditionExpression();

    int condExprType = cond->getExpressionType();
    if(condExprType != BOOL_T) {
        std::stringstream ss;
        ss << "If-statement condition expression has ";
        if(condExprType == ANY_TYPE) {
            ss << "unknown type ";
        } else {
            ss << "type '" << AST::getTypeString(condExprType) << "' ";
        }
        ss << "at " << AST::getSourceLocationString(cond->getSourceLocation()) << ". Expecting type 'bool'.";

        auto id = m_semaAnalyzer.createEvent(ifStatementNode, SemanticAnalyzer::EventType::Error);
        m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
        m_semaAnalyzer.getEvent(id).EventLoc() = cond->getSourceLocation();
    }
}

void TypeChecker::postNodeVisit(AST::AssignmentNode *assignmentNode) {
    /*
     * The value assigned to a variable (this includes variables declared with an initial value)
     * must have the same type as that variable, or a type that can be widened to the correct type.
     */
    const AST::ExpressionNode *rhsExpr = assignmentNode->getExpression();
    const AST::VariableNode *lhsVar = assignmentNode->getVariable();
    int rhsDataType = rhsExpr->getExpressionType();
    int lhsDataType = lhsVar->getExpressionType();

    int resultDataType = ANY_TYPE;
    bool assignmentLegal = true;

    if(lhsDataType == ANY_TYPE && rhsDataType == ANY_TYPE) {
        assignmentLegal = false;

        std::stringstream ss;
        ss << "Variable assignment for '" << lhsVar->getName() << "' at " << AST::getSourceLocationString(assignmentNode->getSourceLocation()) <<
            ", has unknown type on both sides of assignment operator due to pervious errors.";
        
        auto id = m_semaAnalyzer.createEvent(assignmentNode, SemanticAnalyzer::EventType::Error);
        m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
        m_semaAnalyzer.getEvent(id).EventLoc() = assignmentNode->getSourceLocation();
    } else if (lhsDataType == ANY_TYPE && rhsDataType != ANY_TYPE) {
        assignmentLegal = false;

        std::stringstream ss;
        ss << "Variable assignment for '" << lhsVar->getName() << "' at " << AST::getSourceLocationString(assignmentNode->getSourceLocation()) <<
            ", has variable of unknown type at " << AST::getSourceLocationString(lhsVar->getSourceLocation()) << " due to previous error.";
        
        auto id = m_semaAnalyzer.createEvent(assignmentNode, SemanticAnalyzer::EventType::Error);
        m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
        m_semaAnalyzer.getEvent(id).EventLoc() = assignmentNode->getSourceLocation();
    } else if (lhsDataType != ANY_TYPE && rhsDataType == ANY_TYPE) {
        assignmentLegal = false;

        std::stringstream ss;
        ss << "Variable assignment for '" << lhsVar->getName() << "' at " << AST::getSourceLocationString(assignmentNode->getSourceLocation()) <<
            ", has expression of unknown type at " << AST::getSourceLocationString(rhsExpr->getSourceLocation()) << " due to previous error(s).";
        
        auto id = m_semaAnalyzer.createEvent(assignmentNode, SemanticAnalyzer::EventType::Error);
        m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
        m_semaAnalyzer.getEvent(id).EventLoc() = assignmentNode->getSourceLocation();
    } else {
        assert(rhsDataType != ANY_TYPE);
        assert(lhsDataType != ANY_TYPE);

        if(lhsDataType != rhsDataType) {
            assignmentLegal = false;

            std::stringstream ss;
            ss << "Invalid variable assignment for '" << lhsVar->getName() <<
                "' at " << AST::getSourceLocationString(assignmentNode->getSourceLocation()) <<
                ", has expression of non-compatible type '" << (rhsExpr->isConst() ? "const " : "") << AST::getTypeString(rhsDataType) <<
                "' at " << AST::getSourceLocationString(rhsExpr->getSourceLocation()) << ".";
            
            auto id = m_semaAnalyzer.createEvent(assignmentNode, SemanticAnalyzer::EventType::Error);
            m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
            m_semaAnalyzer.getEvent(id).EventLoc() = assignmentNode->getSourceLocation();

            const AST::DeclarationNode *decl = lhsVar->getDeclaration();
            assert(decl != nullptr);
            ss = std::stringstream();
            ss << "Variable '" << lhsVar->getName() << "' at " << getSourceLocationString(lhsVar->getSourceLocation()) <<
                " has type '" << (lhsVar->isConst() ? "const " : "") << AST::getTypeString(lhsDataType) << "', and is declared at " <<
                AST::getSourceLocationString(decl->getSourceLocation()) << ":";

            m_semaAnalyzer.getEvent(id).setUsingReference(true);
            m_semaAnalyzer.getEvent(id).RefMessage() = std::move(ss.str());
            m_semaAnalyzer.getEvent(id).RefLoc() = decl->getSourceLocation();
        }
    }

    if(lhsVar->isConst()) {
        assignmentLegal = false;

        std::stringstream ss;
        ss << "Invalid variable assignment for const-qualified variable '" << lhsVar->getName() << "' at " <<
            AST::getSourceLocationString(assignmentNode->getSourceLocation()) << ".";
        
        auto id = m_semaAnalyzer.createEvent(assignmentNode, SemanticAnalyzer::EventType::Error);
        m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
        m_semaAnalyzer.getEvent(id).EventLoc() = lhsVar->getSourceLocation();

        const AST::DeclarationNode *decl = lhsVar->getDeclaration();
        assert(decl != nullptr);
        ss.str(std::string());
        ss << "Variable '" << lhsVar->getName() << "' at " << getSourceLocationString(lhsVar->getSourceLocation()) <<
            " has type 'const " << AST::getTypeString(lhsDataType) << "', and is declared at " <<
            AST::getSourceLocationString(decl->getSourceLocation()) << ":";

        m_semaAnalyzer.getEvent(id).setUsingReference(true);
        m_semaAnalyzer.getEvent(id).RefMessage() = std::move(ss.str());
        m_semaAnalyzer.getEvent(id).RefLoc() = decl->getSourceLocation();
    }

    if(assignmentLegal) {
        assert(rhsDataType != ANY_TYPE);
        assert(lhsDataType != ANY_TYPE);
        assert(lhsDataType == rhsDataType);

        resultDataType = lhsDataType;
    }

    assignmentNode->setExpressionType(resultDataType);
}

int TypeChecker::inferDataType(int op, int rhsDataType) {
    /* Unary Operator Type Inference */
    /*
     * - s, v Arithmetic
     * ! s, v Logical
     */

    if(rhsDataType == ANY_TYPE) {
        return ANY_TYPE;
    }

    DataTypeCategory typeCateg = getDataTypeCategory(rhsDataType);
    switch(op) {
        case MINUS: {
            if(typeCateg == DataTypeCategory::Arithmetic) {
                return rhsDataType;
            }
            
            if(m_semaAnalyzer.isTempEventCreated()) {
                m_semaAnalyzer.getTempEvent().setUsingReference(true);
                m_semaAnalyzer.getTempEvent().RefMessage() = "Expecting arithmetic type on right-hand side of operator '-'";
            }

            break;
        }
        case NOT: {
            if(typeCateg == DataTypeCategory::Boolean) {
                return rhsDataType;
            }

            if(m_semaAnalyzer.isTempEventCreated()) {
                m_semaAnalyzer.getTempEvent().setUsingReference(true);
                m_semaAnalyzer.getTempEvent().RefMessage() = "Expecting boolean type on right-hand side of operator '!'";
            }

            break;
        }
        default:
            assert(0);
    }

    return ANY_TYPE;
}

int TypeChecker::inferDataType(int op, int lhsDataType, int rhsDataType) {
    /* Binary Operator Type Inference */
    /*
     * +, - ss, vv Arithmetic
     * * ss, vv, sv, vs Arithmetic
     * /, ˆ ss Arithmetic
     * &&, || ss, vv Logical
     * <, <=, >, >= ss Comparison
     * ==, != ss, vv Comparison
     */

    if(lhsDataType == ANY_TYPE || rhsDataType == ANY_TYPE) {
        return ANY_TYPE;
    }

    if(getDataTypeBaseType(lhsDataType) != getDataTypeBaseType(rhsDataType)) {
        /*
         * Both operands of a binary operator must have exactly the same base type (e.g. it
         * is valid to multiple an int and an ivec3)
         */

        if(m_semaAnalyzer.isTempEventCreated()) {
            m_semaAnalyzer.getTempEvent().setUsingReference(true);
            std::stringstream ss;
            ss << "Expecting operands on both sides of operator '";
            ss << AST::getOperatorString(op);
            ss << "' to have same base type, but they are '";
            ss << AST::getTypeString(lhsDataType);
            ss << "' and '";
            ss << AST::getTypeString(rhsDataType);
            ss << "'.";
            m_semaAnalyzer.getTempEvent().RefMessage() = std::move(ss.str());
        }

        return ANY_TYPE;
    }

    int lhsTypeOrder = getDataTypeOrder(lhsDataType);
    int rhsTypeOrder = getDataTypeOrder(rhsDataType);
    if(lhsTypeOrder > 1 && rhsTypeOrder > 1) {
       if(lhsTypeOrder != rhsTypeOrder) {
            /*
            * If both arguments to a binary operator are vectors, then they must be vectors of the
            * same order. For example, it is not valid to add an ivec2 with an ivec3
            */

            if(m_semaAnalyzer.isTempEventCreated()) {
                m_semaAnalyzer.getTempEvent().setUsingReference(true);
                std::stringstream ss;
                ss << "Expecting vector operands on both sides of operator to have same order, but they are '";
                ss << AST::getTypeString(lhsDataType);
                ss << "' and '";
                ss << AST::getTypeString(rhsDataType);
                ss << "'.";
                m_semaAnalyzer.getTempEvent().RefMessage() = std::move(ss.str());
            }

           return ANY_TYPE;
       }
    }

    DataTypeCategory lhsTypeCateg = getDataTypeCategory(lhsDataType);
    DataTypeCategory rhsTypeCateg = getDataTypeCategory(rhsDataType);
    switch(op) {
        case PLUS:
        case MINUS: {
            if(lhsTypeCateg == DataTypeCategory::Arithmetic && rhsTypeCateg == DataTypeCategory::Arithmetic) {
                if(lhsTypeOrder == rhsTypeOrder) {
                    // Same base type
                    // Both arithmetic type
                    // Same type order
                    assert(lhsDataType == rhsDataType);
                    return lhsDataType;
                }
            }

            if(m_semaAnalyzer.isTempEventCreated()) {
                m_semaAnalyzer.getTempEvent().setUsingReference(true);
                std::stringstream ss;
                ss << "Expecting operands on both sides of operator '";
                ss << AST::getOperatorString(op);
                ss << "' to have arithmetic type and same order, but they are '";
                ss << AST::getTypeString(lhsDataType);
                ss << "' and '";
                ss << AST::getTypeString(rhsDataType);
                ss << "'.";
                m_semaAnalyzer.getTempEvent().RefMessage() = std::move(ss.str());
            }

            break;
        }

        case TIMES: {
            if(lhsTypeCateg == DataTypeCategory::Arithmetic && rhsTypeCateg == DataTypeCategory::Arithmetic) {
                if(lhsTypeOrder > rhsTypeOrder) {
                    // Same base type
                    // Both arithmetic type
                    // If both vector, same type order; otherwise, any order combination eg. ss and sv
                    // Return the larger order one
                    return lhsDataType;
                } else {
                    return rhsDataType;
                }
            }

            if(m_semaAnalyzer.isTempEventCreated()) {
                m_semaAnalyzer.getTempEvent().setUsingReference(true);
                std::stringstream ss;
                ss << "Expecting operands on both sides of operator '";
                ss << AST::getOperatorString(op);
                ss << "' to have arithmetic type, but they are '";
                ss << AST::getTypeString(lhsDataType);
                ss << "' and '";
                ss << AST::getTypeString(rhsDataType);
                ss << "'.";
                m_semaAnalyzer.getTempEvent().RefMessage() = std::move(ss.str());
            }

            break;
        }

        case SLASH:
        case EXP: {
            if(lhsTypeCateg == DataTypeCategory::Arithmetic && rhsTypeCateg == DataTypeCategory::Arithmetic) {
                if(lhsTypeOrder == 1 && rhsTypeOrder == 1) {
                    // Same base type
                    // Both arithmetic type
                    // Both scalar type
                    assert(lhsDataType == rhsDataType);
                    return lhsDataType;
                }
            }

            if(m_semaAnalyzer.isTempEventCreated()) {
                m_semaAnalyzer.getTempEvent().setUsingReference(true);
                std::stringstream ss;
                ss << "Expecting operands on both sides of operator '";
                ss << AST::getOperatorString(op);
                ss << "' to be scalar and have arithmetic type, but they are '";
                ss << AST::getTypeString(lhsDataType);
                ss << "' and '";
                ss << AST::getTypeString(rhsDataType);
                ss << "'.";
                m_semaAnalyzer.getTempEvent().RefMessage() = std::move(ss.str());
            }

            break;
        }

        case AND:
        case OR: {
            if(lhsTypeCateg == DataTypeCategory::Boolean && rhsTypeCateg == DataTypeCategory::Boolean) {
                if(lhsTypeOrder == rhsTypeOrder) {
                    // Same base type
                    // Both boolean type
                    // Same type order
                    assert(lhsDataType == rhsDataType);
                    return lhsDataType;
                }
            }

            if(m_semaAnalyzer.isTempEventCreated()) {
                m_semaAnalyzer.getTempEvent().setUsingReference(true);
                std::stringstream ss;
                ss << "Expecting operands on both sides of operator '";
                ss << AST::getOperatorString(op);
                ss << "' to have boolean type and same order, but they are '";
                ss << AST::getTypeString(lhsDataType);
                ss << "' and '";
                ss << AST::getTypeString(rhsDataType);
                ss << "'.";
                m_semaAnalyzer.getTempEvent().RefMessage() = std::move(ss.str());
            }

            break;
        }

        case LSS:
        case LEQ:
        case GTR:
        case GEQ: {
            if(lhsTypeCateg == DataTypeCategory::Arithmetic && rhsTypeCateg == DataTypeCategory::Arithmetic) {
                if(lhsTypeOrder == 1 && rhsTypeOrder == 1) {
                    // Same base type
                    // Both arithmetic type
                    // Both scalar type
                    assert(lhsDataType == rhsDataType);
                    return lhsDataType;
                }
            }

            if(m_semaAnalyzer.isTempEventCreated()) {
                m_semaAnalyzer.getTempEvent().setUsingReference(true);
                std::stringstream ss;
                ss << "Expecting operands on both sides of operator '";
                ss << AST::getOperatorString(op);
                ss << "' to be scalar and have arithmetic type, but they are '";
                ss << AST::getTypeString(lhsDataType);
                ss << "' and '";
                ss << AST::getTypeString(rhsDataType);
                ss << "'.";
                m_semaAnalyzer.getTempEvent().RefMessage() = std::move(ss.str());
            }

            break;
        }

        case EQL:
        case NEQ: {
            if(lhsTypeCateg == DataTypeCategory::Arithmetic && rhsTypeCateg == DataTypeCategory::Arithmetic) {
                if(lhsTypeOrder == rhsTypeOrder) {
                    // Same base type
                    // Both arithmetic type
                    // Same type order
                    assert(lhsDataType == rhsDataType);
                    return lhsDataType;
                }
            }

            if(m_semaAnalyzer.isTempEventCreated()) {
                m_semaAnalyzer.getTempEvent().setUsingReference(true);
                std::stringstream ss;
                ss << "Expecting operands on both sides of operator '";
                ss << AST::getOperatorString(op);
                ss << "' to have arithmetic type and same order, but they are '";
                ss << AST::getTypeString(lhsDataType);
                ss << "' and '";
                ss << AST::getTypeString(rhsDataType);
                ss << "'.";
                m_semaAnalyzer.getTempEvent().RefMessage() = std::move(ss.str());
            }

            break;
        }

        default:
            assert(0);
    }

    return ANY_TYPE;
}

} /* END NAMESPACE */

int semantic_check(node * ast) {
    ST::SymbolTable symbolTable;
    SEMA::SemanticAnalyzer semaAnalyzer;
    SEMA::SourceLocation sourceLocation(inputFile);

    /* Create Predefined Variables */
    SEMA::PredefinedVariableCreater predefinedVariableCreater;
    static_cast<AST::ASTNode *>(ast)->visit(predefinedVariableCreater);

    /* Construct Symbol Tree */
    SEMA::SymbolDeclVisitor symbolDeclVisitor(symbolTable, semaAnalyzer);
    static_cast<AST::ASTNode *>(ast)->visit(symbolDeclVisitor);
    // symbolTable.printScopeLeaves();

    /* Type Checker */
    SEMA::TypeChecker typeChecker(symbolTable, semaAnalyzer);
    static_cast<AST::ASTNode *>(ast)->visit(typeChecker);
    // symbolTable.printSymbolReference();
    ast_print(ast);

    /* Analyzing Semantic Analysis Result */
    int numEvents = semaAnalyzer.getNumberEvents();
    if(numEvents != 0) {
        printf("\n");
    }
    for(int id = 0; id < numEvents; id++) {
        semaAnalyzer.printEvent(id, sourceLocation);
    }
    if(numEvents != 0) {
        printf("--------------------------------------------------------------------------\n");
    }

    return 1;
}