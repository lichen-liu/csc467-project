#include "semantic.h"

#include "ast.h"
#include "symbol.h"

#include "common.h"
#include "parser.tab.h"

#include <algorithm>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <array>
#include <algorithm>
#include <sstream>

#include <cmath>
#include <cassert>

///////////////////////////////////////
//#define NORMAL_MODE
///////////////////////////////////////

namespace SEMA{ /* START NAMESPACE */

class SourceContext {
    private:
        std::vector<std::string> m_sourceFile;

    public:
        SourceContext(FILE *f);
    
    public:
        const std::vector<std::string> &getLines() const { return m_sourceFile; }
        const std::string &getLine(int line) const { return m_sourceFile.at(line - 1); }
};

SourceContext::SourceContext(FILE *f) {
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

    private:
        bool m_colorPrintEnabled = true;
        FILE *m_out = stdout;
    
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

    private:
        void printEventNoColor(EventID eventID, const SourceContext &sourceContext) const;
        void printEventColor(EventID eventID, const SourceContext &sourceContext) const;
    
    public:
        bool isColorPrintEnabled() const { return m_colorPrintEnabled; }
        void setColorPrintEnabled(bool flag) { m_colorPrintEnabled = flag; }
        void setOutput(FILE *out) { m_out = out; }
        FILE *getOutput() const { return m_out; }
        void printEvent(EventID eventID, const SourceContext &sourceContext) const;

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

void SemanticAnalyzer::printEventNoColor(EventID eventID, const SourceContext &sourceContext) const {
    static const std::string tenSpace(10, ' ');
    static const std::string sevenSpace(7, ' ');

    const Event &event = getEvent(eventID);
    const AST::SourceLocation &eventLoc = event.EventLoc();
    
    fprintf(m_out, "--------------------------------------------------------------------------\n");

    // Print event message
    std::string header;
    if(event.getEventType() == EventType::Error) {
        header = "Error";
    } else {
        header = "Warning";
    }
    fprintf(m_out, "%s-%d", header.c_str(), eventID);
    fprintf(m_out, ": %s\n", event.Message().c_str());

    // Print event location info
    if(AST::SourceLocation() != eventLoc) {
        if(eventLoc.firstLine == eventLoc.lastLine) {
            const std::string &line = sourceContext.getLine(eventLoc.firstLine);
            fprintf(m_out, "%7d:%s", eventLoc.firstLine, tenSpace.c_str());
            fprintf(m_out, "%s\n", line.c_str());
            fprintf(m_out, "%s %s", sevenSpace.c_str(), tenSpace.c_str());
            for(int colNumber = 0; colNumber < line.length(); colNumber++) {
                if(colNumber >= eventLoc.firstColumn - 1 && colNumber < eventLoc.lastColumn - 1) {
                    fprintf(m_out, "^");
                } else {
                    fprintf(m_out, " ");
                }
            }
            fprintf(m_out, "\n");
        } else {
            for(int lineNumber = eventLoc.firstLine; lineNumber <= eventLoc.lastLine; lineNumber++) {
                const std::string &line = sourceContext.getLine(lineNumber);
                fprintf(m_out, "%7d:%s", lineNumber, tenSpace.c_str());
                fprintf(m_out, "%s\n", line.c_str());
            }
        }
    }

    // Print reference location info and reference message
    if(event.isUsingReference()) {
        const AST::SourceLocation &refLoc = event.RefLoc();

        fprintf(m_out, "\n");

        // Print reference message
        fprintf(m_out, "Info");
        fprintf(m_out, ": %s\n", event.RefMessage().c_str());

        // Print reference location info
        if(AST::SourceLocation() != refLoc) {
            if(refLoc.firstLine == refLoc.lastLine) {
                const std::string &line = sourceContext.getLine(refLoc.firstLine);
                fprintf(m_out, "%7d:%s", refLoc.firstLine, tenSpace.c_str());
                fprintf(m_out, "%s\n", line.c_str());
                fprintf(m_out, "%s %s", sevenSpace.c_str(), tenSpace.c_str());
                for(int colNumber = 0; colNumber < line.length(); colNumber++) {
                    if(colNumber >= refLoc.firstColumn - 1 && colNumber < refLoc.lastColumn - 1) {
                        fprintf(m_out, "~");
                    } else {
                        fprintf(m_out, " ");
                    }
                }
                fprintf(m_out, "\n");
            } else {
                for(int lineNumber = refLoc.firstLine; lineNumber <= refLoc.lastLine; lineNumber++) {
                    const std::string &line = sourceContext.getLine(lineNumber);
                    fprintf(m_out, "%7d:%s", lineNumber, tenSpace.c_str());
                    fprintf(m_out, "%s\n", line.c_str());
                }
            }
        }
    }
}

void SemanticAnalyzer::printEventColor(EventID eventID, const SourceContext &sourceContext) const {
    static const std::string tenSpace(10, ' ');
    static const std::string sevenSpace(7, ' ');

    const Event &event = getEvent(eventID);
    const AST::SourceLocation &eventLoc = event.EventLoc();
    
    printf("--------------------------------------------------------------------------\n");

    // Print event message
    if(event.getEventType() == EventType::Error) {
        printf("\033[1;31mError-%d\033[0m", eventID);
    } else {
        printf("\033[1;33mWarning-%d\033[0m", eventID);
    }
    printf("\033[1;39m: %s\033[0m\n", event.Message().c_str());

    // Print event location info
    if(AST::SourceLocation() != eventLoc) {
        if(eventLoc.firstLine == eventLoc.lastLine) {
            const std::string &line = sourceContext.getLine(eventLoc.firstLine);
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
            printf("\n");
        } else {
            for(int lineNumber = eventLoc.firstLine; lineNumber <= eventLoc.lastLine; lineNumber++) {
                const std::string &line = sourceContext.getLine(lineNumber);
                printf("\033[0;32m%7d:%s\033[0m", lineNumber, tenSpace.c_str());
                printf("\033[1;37m%s\033[0m\n", line.c_str());
            }
        }
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
                const std::string &line = sourceContext.getLine(refLoc.firstLine);
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
                printf("\n");
            } else {
                for(int lineNumber = refLoc.firstLine; lineNumber <= refLoc.lastLine; lineNumber++) {
                    const std::string &line = sourceContext.getLine(lineNumber);
                    printf("\033[0;32m%7d:%s\033[0m", lineNumber, tenSpace.c_str());
                    printf("\033[1;37m%s\033[0m\n", line.c_str());
                }
            }
        }
    }
}

void SemanticAnalyzer::printEvent(EventID eventID, const SourceContext &sourceContext) const {
    if(isColorPrintEnabled()) {
        printEventColor(eventID, sourceContext);
    } else {
        printEventNoColor(eventID, sourceContext);
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
                ss << "Duplicate declaration of '" << declarationNode->getQualifierString() << declarationNode->getTypeString() <<
                    " " << declarationNode->getName() << "' at " << declarationNode->getSourceLocationString() << ". " <<
                    "Previously declared at " << redecl->getSourceLocationString() << ".";

                auto id = m_semaAnalyzer.createEvent(declarationNode, SemanticAnalyzer::EventType::Error);
                m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
                m_semaAnalyzer.getEvent(id).EventLoc() = declarationNode->getSourceLocation();

                m_semaAnalyzer.getEvent(id).setUsingReference(true);
                if(redecl->isOrdinaryType()) {
                    m_semaAnalyzer.getEvent(id).RefMessage() = "Previously declared here:";
                    m_semaAnalyzer.getEvent(id).RefLoc() = redecl->getSourceLocation();
                } else {
                    m_semaAnalyzer.getEvent(id).RefMessage() = "Predefined Variable: '" + redecl->getQualifierString() +
                        redecl->getTypeString() + " " + redecl->getName() + "'.";
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

/* Finds Write-Only Type in Expressions Only */
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
        /* Do not traverse into */
        virtual void nodeVisit(AST::IndexingNode *indexingNode) {}
        virtual void nodeVisit(AST::StatementNode *statementNode) {}
        virtual void nodeVisit(AST::StatementsNode *statementsNode) {}
        virtual void nodeVisit(AST::DeclarationNode *declarationNode) {}
        virtual void nodeVisit(AST::DeclarationsNode *declarationsNode) {}
        virtual void nodeVisit(AST::IfStatementNode *ifStatementNode) {}
        virtual void nodeVisit(AST::AssignmentNode *assignmentNode) {}
        virtual void nodeVisit(AST::StallStatementNode *stallStatementNode) {}
        virtual void nodeVisit(AST::NestedScopeNode *nestedScopeNode) {}
        virtual void nodeVisit(AST::ScopeNode *scopeNode) {}

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

        int m_ifScopeCount = 0;
    public:
        TypeChecker(ST::SymbolTable &symbolTable, SEMA::SemanticAnalyzer &semaAnalyzer):
            m_symbolTable(symbolTable), m_semaAnalyzer(semaAnalyzer) {}
    
    private:
        virtual void preNodeVisit(AST::IdentifierNode *identifierNode);
        virtual void preNodeVisit(AST::IfStatementNode *ifStatementNode);

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
        ss << "Missing declaration for symbol '" << identifierNode->getName() <<
        "' at " << identifierNode->getSourceLocationString() << ".";

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

void TypeChecker::preNodeVisit(AST::IfStatementNode *ifStatementNode) {
    assert(m_ifScopeCount >= 0);
    m_ifScopeCount++;
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
            ss << "Operand in unary expression at " << unaryExpressionNode->getSourceLocationString() <<
                " has write-only Result type at " << rhsExpr->getSourceLocationString() << ".";

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
            ss << "Operand in unary expression at " << unaryExpressionNode->getSourceLocationString() <<
                " has non-compatible type at " << rhsExpr->getSourceLocationString() << ".";

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

    if(identifierDataType != ANY_TYPE) {
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
                    "' of type '" << identifier->getDeclaration()->getQualifierString() << AST::getTypeString(identifierDataType) << "' at " <<
                    indexingNode->getSourceLocationString() << ".";

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
                "' of type '" << identifier->getDeclaration()->getQualifierString() << AST::getTypeString(identifierDataType) << "' at " <<
                indexingNode->getSourceLocationString() << ".";

            auto id = m_semaAnalyzer.createEvent(indexingNode, SemanticAnalyzer::EventType::Error);
            m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
            m_semaAnalyzer.getEvent(id).EventLoc() = indexingNode->getSourceLocation();
        }
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
                functionNode->getSourceLocationString() << ".";

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
                functionNode->getSourceLocationString() << ".";

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
                functionNode->getSourceLocationString() << ".";

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
            ss << "Arguments in function call at " << functionNode->getSourceLocationString() <<
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
            constructorNode->getSourceLocationString() << ".";

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
            ss << "Arguments in constructor call at " << constructorNode->getSourceLocationString() <<
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
    AST::ExpressionNode *initExpr = declarationNode->getExpression();
    if(declarationNode->isConst()) {
        if(initExpr == nullptr) {
            std::stringstream ss;
            ss << "Const qualified variable 'const " << declarationNode->getTypeString() << " " << declarationNode->getName() <<
                "' is missing initialization at " << declarationNode->getSourceLocationString() << ".";

            auto id = m_semaAnalyzer.createEvent(declarationNode, SemanticAnalyzer::EventType::Error);
            m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
            m_semaAnalyzer.getEvent(id).EventLoc() = declarationNode->getSourceLocation();

        } else if (!initExpr->isConst()) {
            std::stringstream ss;
            ss << "Const qualified variable 'const " << declarationNode->getTypeString() << " " << declarationNode->getName() <<
                "' is initialized to a non-const qualified expression at " << initExpr->getSourceLocationString() << ".";

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
            ss << "Variable declaration of '" << declarationNode->getQualifierString() << AST::getTypeString(lhsDataType) << " " << declarationNode->getName() <<
                "' at " << declarationNode->getSourceLocationString()  << ", is initialized to an unknown type at " <<
                initExpr->getSourceLocationString() << " due to previous error(s).";
            
            auto id = m_semaAnalyzer.createEvent(declarationNode, SemanticAnalyzer::EventType::Error);
            m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
            m_semaAnalyzer.getEvent(id).EventLoc() = declarationNode->getSourceLocation();
        } else {
            // Firstly, check for Write-Only
            WriteOnlyFinder writeOnlyFinder;
            initExpr->visit(writeOnlyFinder);
            const std::vector<const AST::VariableNode *> &writeOnlyVars = writeOnlyFinder.getWriteOnlyVars();
            if(!writeOnlyVars.empty()) {
                std::stringstream ss;
                ss << "Variable declaration of '" << declarationNode->getQualifierString() << AST::getTypeString(lhsDataType) << " " << declarationNode->getName() <<
                    "' at " << declarationNode->getSourceLocationString()  << ", is initialized to a write-only Result type at " <<
                    initExpr->getSourceLocationString() << ".";

                auto id = m_semaAnalyzer.createEvent(declarationNode, SemanticAnalyzer::EventType::Error);
                m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
                m_semaAnalyzer.getEvent(id).EventLoc() = declarationNode->getSourceLocation();

                m_semaAnalyzer.getEvent(id).setUsingReference(true);
                m_semaAnalyzer.getEvent(id).RefMessage() = "The first write-only Result variable is '" + writeOnlyVars.front()->getName() + "':";
                m_semaAnalyzer.getEvent(id).RefLoc() = writeOnlyVars.front()->getSourceLocation();
            }

            // Secondly, Type check
            if(lhsDataType != rhsDataType) {
                std::stringstream ss;
                ss << "Variable declaration of '" << declarationNode->getQualifierString() << AST::getTypeString(lhsDataType) << " " << declarationNode->getName() <<
                    "' at " << declarationNode->getSourceLocationString()  << ", is initialized to a noncompatible type '"<< AST::getTypeString(rhsDataType) <<
                    "' at " << initExpr->getSourceLocationString() << ".";

                auto id = m_semaAnalyzer.createEvent(declarationNode, SemanticAnalyzer::EventType::Error);
                m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
                m_semaAnalyzer.getEvent(id).EventLoc() = declarationNode->getSourceLocation();
            }
        }
    }
}

void TypeChecker::postNodeVisit(AST::IfStatementNode *ifStatementNode) {
    /*
     * The expression that determines which branch of an if statement should be taken must
     * have the type bool (not bvec).
     */
    AST::ExpressionNode *cond = ifStatementNode->getConditionExpression();
    int condExprType = cond->getExpressionType();

    // Firstly, check for Write-Only for condition expression
    if(condExprType != ANY_TYPE) {
        WriteOnlyFinder writeOnlyFinder;
        cond->visit(writeOnlyFinder);
        const std::vector<const AST::VariableNode *> &writeOnlyVars = writeOnlyFinder.getWriteOnlyVars();
        if(!writeOnlyVars.empty()) {
            std::stringstream ss;
            ss << "If-statement condition expression has write-only Result type at " <<
                cond->getSourceLocationString() << ".";

            auto id = m_semaAnalyzer.createEvent(ifStatementNode, SemanticAnalyzer::EventType::Error);
            m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
            m_semaAnalyzer.getEvent(id).EventLoc() = cond->getSourceLocation();

            m_semaAnalyzer.getEvent(id).setUsingReference(true);
            m_semaAnalyzer.getEvent(id).RefMessage() = "The first write-only Result variable is '" + writeOnlyVars.front()->getName() + "':";
            m_semaAnalyzer.getEvent(id).RefLoc() = writeOnlyVars.front()->getSourceLocation();
        }
    }

    if(condExprType != BOOL_T) {
        std::stringstream ss;
        ss << "If-statement condition expression has ";
        if(condExprType == ANY_TYPE) {
            ss << "unknown type ";
        } else {
            ss << "type '" << AST::getTypeString(condExprType) << "' ";
        }
        ss << "at " << cond->getSourceLocationString() << ". Expecting type 'bool'.";

        auto id = m_semaAnalyzer.createEvent(ifStatementNode, SemanticAnalyzer::EventType::Error);
        m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
        m_semaAnalyzer.getEvent(id).EventLoc() = cond->getSourceLocation();
    }

    m_ifScopeCount--;
    assert(m_ifScopeCount >= 0);
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
        ss << "Variable assignment for '" << lhsVar->getName() << "' at " << assignmentNode->getSourceLocationString() <<
            ", has unknown type on both sides of assignment operator due to pervious errors.";
        
        auto id = m_semaAnalyzer.createEvent(assignmentNode, SemanticAnalyzer::EventType::Error);
        m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
        m_semaAnalyzer.getEvent(id).EventLoc() = assignmentNode->getSourceLocation();
    } else if (lhsDataType == ANY_TYPE && rhsDataType != ANY_TYPE) {
        assignmentLegal = false;

        std::stringstream ss;
        ss << "Variable assignment for '" << lhsVar->getName() << "' at " << assignmentNode->getSourceLocationString() <<
            ", has variable of unknown type at " << lhsVar->getSourceLocationString() << " due to previous error.";
        
        auto id = m_semaAnalyzer.createEvent(assignmentNode, SemanticAnalyzer::EventType::Error);
        m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
        m_semaAnalyzer.getEvent(id).EventLoc() = assignmentNode->getSourceLocation();
    } else if (lhsDataType != ANY_TYPE && rhsDataType == ANY_TYPE) {
        assignmentLegal = false;

        std::stringstream ss;
        ss << "Variable assignment for '" << lhsVar->getName() << "' at " << assignmentNode->getSourceLocationString() <<
            ", has expression of unknown type at " << rhsExpr->getSourceLocationString() << " due to previous error(s).";
        
        auto id = m_semaAnalyzer.createEvent(assignmentNode, SemanticAnalyzer::EventType::Error);
        m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
        m_semaAnalyzer.getEvent(id).EventLoc() = assignmentNode->getSourceLocation();
    } else {
        assert(rhsDataType != ANY_TYPE);
        assert(lhsDataType != ANY_TYPE);

        // Firstly, check whether lhs is Read-Only
        if(lhsVar->isReadOnly()) {
            std::stringstream ss;
            ss << "Invalid variable assignment for Read-Only variable '"<< lhsVar->getName() <<
                "' at " << assignmentNode->getSourceLocationString() << ".";
            
            auto id = m_semaAnalyzer.createEvent(assignmentNode, SemanticAnalyzer::EventType::Error);
            m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
            m_semaAnalyzer.getEvent(id).EventLoc() = assignmentNode->getSourceLocation();

            m_semaAnalyzer.getEvent(id).setUsingReference(true);

            ss = std::stringstream();
            const AST::DeclarationNode *decl = lhsVar->getDeclaration();
            assert(decl != nullptr);
            ss << "Predefined Variable: '" << decl->getQualifierString() << lhsVar->getExpressionTypeString() << " " << decl->getName() + "'.";
            m_semaAnalyzer.getEvent(id).RefMessage() = std::move(ss.str());
            m_semaAnalyzer.getEvent(id).RefLoc() = lhsVar->getSourceLocation();
        }

        // Secondly, check whether lhs is Write-Only and is not in if-else-statement scope
        if(m_ifScopeCount > 0 && lhsVar->isWriteOnly()) {
            std::stringstream ss;
            ss << "Invalid variable assignment for Write-Only Result variable '"<< lhsVar->getName() <<
                "' in the scope of an if or else statement at " << assignmentNode->getSourceLocationString() << ".";
            
            auto id = m_semaAnalyzer.createEvent(assignmentNode, SemanticAnalyzer::EventType::Error);
            m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
            m_semaAnalyzer.getEvent(id).EventLoc() = assignmentNode->getSourceLocation();

            m_semaAnalyzer.getEvent(id).setUsingReference(true);

            ss = std::stringstream();
            const AST::DeclarationNode *decl = lhsVar->getDeclaration();
            assert(decl != nullptr);
            ss << "Predefined Variable: 'result " << lhsVar->getExpressionTypeString() << " " << decl->getName() + "'.";
            m_semaAnalyzer.getEvent(id).RefMessage() = std::move(ss.str());
            m_semaAnalyzer.getEvent(id).RefLoc() = lhsVar->getSourceLocation();
        }

        // Thirdly, check for type
        if(lhsDataType != rhsDataType) {
            assignmentLegal = false;

            std::stringstream ss;
            ss << "Invalid variable assignment for '" << lhsVar->getName() <<
                "' at " << assignmentNode->getSourceLocationString() <<
                ", has expression of non-compatible type '" << rhsExpr->getExpressionQualifierString() << AST::getTypeString(rhsDataType) <<
                "' at " << rhsExpr->getSourceLocationString() << ".";
            
            auto id = m_semaAnalyzer.createEvent(assignmentNode, SemanticAnalyzer::EventType::Error);
            m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
            m_semaAnalyzer.getEvent(id).EventLoc() = assignmentNode->getSourceLocation();

            const AST::DeclarationNode *decl = lhsVar->getDeclaration();
            assert(decl != nullptr);
            ss = std::stringstream();
            m_semaAnalyzer.getEvent(id).setUsingReference(true);
            
            if(lhsVar->isOrdinaryType()) {
                ss << "Variable '" << lhsVar->getName() << "' at " << lhsVar->getSourceLocationString() <<
                    " has type '" << decl->getQualifierString() << AST::getTypeString(lhsDataType) << "', and is declared at " <<
                    decl->getSourceLocationString() << ":";

                m_semaAnalyzer.getEvent(id).RefMessage() = std::move(ss.str());
                m_semaAnalyzer.getEvent(id).RefLoc() = decl->getSourceLocation();
            } else {
                // predefined variables
                ss << "Variable '" << lhsVar->getName() << "' at " << lhsVar->getSourceLocationString() <<
                    " has type '" << decl->getQualifierString() << AST::getTypeString(lhsDataType) << "', and is a predefined variable.";

                m_semaAnalyzer.getEvent(id).RefMessage() = std::move(ss.str());
            }
        }
    }

    if(lhsVar->isOrdinaryType() && lhsVar->isConst()) {
        assignmentLegal = false;

        std::stringstream ss;
        ss << "Invalid variable assignment for const-qualified variable '" << lhsVar->getName() << "' at " <<
            assignmentNode->getSourceLocationString() << ".";
        
        auto id = m_semaAnalyzer.createEvent(assignmentNode, SemanticAnalyzer::EventType::Error);
        m_semaAnalyzer.getEvent(id).Message() = std::move(ss.str());
        m_semaAnalyzer.getEvent(id).EventLoc() = lhsVar->getSourceLocation();

        const AST::DeclarationNode *decl = lhsVar->getDeclaration();
        assert(decl != nullptr);
        ss.str(std::string());
        ss << "Variable '" << lhsVar->getName() << "' at " << lhsVar->getSourceLocationString() <<
            " has type '" << decl->getQualifierString() << AST::getTypeString(lhsDataType) << "', and is declared at " <<
            decl->getSourceLocationString() << ":";

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

class DataContainer {
    private:
        const int m_type;
    private:
        const int m_typeBase;
        const int m_typeOrder;
        union ValueUnion {
            std::array<int, 4> intVal;
            std::array<float, 4> floatVal;
            std::array<bool, 4> boolVal;
        } m_value = {0};
    public:
        using IntArrayCIt = std::array<int, 4>::const_iterator;
        using FloatArrayCIt = std::array<float, 4>::const_iterator;
        using BoolArrayCIt = std::array<bool, 4>::const_iterator;

    public:
        DataContainer(int type):
            m_type(type), m_typeBase(getDataTypeBaseType(m_type)), m_typeOrder(getDataTypeOrder(m_type)) {}
        DataContainer(const DataContainer& other):
            m_type(other.m_type), m_typeBase(other.m_typeBase), m_typeOrder(other.m_typeOrder), m_value(other.m_value) {}
    
    public:
        int getType() const { return m_type; }
        int getTypeBase() const { return m_typeBase; }
        int getTypeOrder() const { return m_typeOrder; }

    public:
        std::array<int, 4> &getIntVal() { assert(m_typeBase == INT_T); return m_value.intVal; }
        std::array<float, 4> &getFloatVal() { assert(m_typeBase == FLOAT_T); return m_value.floatVal; }
        std::array<bool, 4> &getBoolVal() { assert(m_typeBase == BOOL_T); return m_value.boolVal; }
        const std::array<int, 4> &getIntVal() const { assert(m_typeBase == INT_T); return m_value.intVal; }
        const std::array<float, 4> &getFloatVal() const { assert(m_typeBase == FLOAT_T); return m_value.floatVal; }
        const std::array<bool, 4> &getBoolVal() const { assert(m_typeBase == BOOL_T); return m_value.boolVal; }

    public:
        IntArrayCIt getIntValBegin() const { assert(m_typeBase == INT_T); return m_value.intVal.begin(); }
        FloatArrayCIt getFloatValBegin() const { assert(m_typeBase == FLOAT_T); return m_value.floatVal.begin(); }
        BoolArrayCIt getBoolValBegin() const { assert(m_typeBase == BOOL_T); return m_value.boolVal.begin(); }
        IntArrayCIt getIntValEnd() const { assert(m_typeBase == INT_T); return m_value.intVal.begin() + m_typeOrder; }
        FloatArrayCIt getFloatValEnd() const { assert(m_typeBase == FLOAT_T); return m_value.floatVal.begin() + m_typeOrder; }
        BoolArrayCIt getBoolValEnd() const { assert(m_typeBase == BOOL_T); return m_value.boolVal.begin() + m_typeOrder; }

    public:
        DataContainer& operator= (const DataContainer &rhs);
    
    public:
        DataContainer getSlice(int idx);

    public:
        AST::ExpressionNode *createASTExpr() const;
};

DataContainer operator+(const DataContainer &lhs, const DataContainer &rhs) {
    /*
     * +, - ss, vv Arithmetic
     */
    assert(lhs.getType() == rhs.getType());
    assert(getDataTypeCategory(lhs.getType()) == DataTypeCategory::Arithmetic);

    DataContainer result(lhs.getType());
    switch(lhs.getTypeBase()) {
        case INT_T: {
            std::transform(lhs.getIntValBegin(), lhs.getIntValEnd(), rhs.getIntValBegin(), result.getIntVal().begin(), 
                [](int a, int b) -> int {
                    return a + b;
                }
                );
            break;
        }
        case FLOAT_T: {
            std::transform(lhs.getFloatValBegin(), lhs.getFloatValEnd(), rhs.getFloatValBegin(), result.getFloatVal().begin(), 
                [](float a, float b) -> float {
                    return a + b;
                }
                );
            break;
        }
        case BOOL_T:
            assert(0);
        default:
            assert(0);
    }

    return result;
}

DataContainer operator-(const DataContainer &lhs, const DataContainer &rhs) {
    /*
     * +, - ss, vv Arithmetic
     */
    assert(lhs.getType() == rhs.getType());
    assert(getDataTypeCategory(lhs.getType()) == DataTypeCategory::Arithmetic);

    DataContainer result(lhs.getType());
    switch(lhs.getTypeBase()) {
        case INT_T: {
            std::transform(lhs.getIntValBegin(), lhs.getIntValEnd(), rhs.getIntValBegin(), result.getIntVal().begin(), 
                [](int a, int b) -> int {
                    return a - b;
                }
                );
            break;
        }
        case FLOAT_T: {
            std::transform(lhs.getFloatValBegin(), lhs.getFloatValEnd(), rhs.getFloatValBegin(), result.getFloatVal().begin(), 
                [](float a, float b) -> float {
                    return a - b;
                }
                );
            break;
        }
        case BOOL_T:
            assert(0);
        default:
            assert(0);
    }

    return result;
}

DataContainer operator*(const DataContainer &lhs, const DataContainer &rhs) {
    /*
     * * ss, vv, sv, vs Arithmetic
     */
    assert(lhs.getTypeBase() == rhs.getTypeBase());
    assert(getDataTypeCategory(lhs.getType()) == DataTypeCategory::Arithmetic);

    if(lhs.getType() == rhs.getType()) {
        /* ss, vv */
        DataContainer result(lhs.getType());
        switch(lhs.getTypeBase()) {
            case INT_T: {
                std::transform(lhs.getIntValBegin(), lhs.getIntValEnd(), rhs.getIntValBegin(), result.getIntVal().begin(), 
                    [](int a, int b) -> int {
                        return a * b;
                    }
                    );
                break;
            }
            case FLOAT_T: {
                std::transform(lhs.getFloatValBegin(), lhs.getFloatValEnd(), rhs.getFloatValBegin(), result.getFloatVal().begin(), 
                    [](float a, float b) -> float {
                        return a * b;
                    }
                    );
                break;
            }
            case BOOL_T:
                assert(0);
            default:
                assert(0);
        }

        return result;
    } else {
        /* sv, vs */
        assert(lhs.getTypeOrder() == 1 || rhs.getTypeOrder() == 1);
        const DataContainer &vec = lhs.getTypeOrder() > 1 ? lhs : rhs;
        const DataContainer &scal = lhs.getTypeOrder() > 1 ? rhs : lhs;
        DataContainer result(vec.getType());

        switch(vec.getTypeBase()) {
            case INT_T: {
                std::transform(vec.getIntValBegin(), vec.getIntValEnd(), result.getIntVal().begin(), 
                    [&scal](int a) -> int {
                        return a * scal.getIntVal()[0];
                    }
                    );
                break;
            }
            case FLOAT_T: {
                std::transform(vec.getFloatValBegin(), vec.getFloatValEnd(), result.getFloatVal().begin(), 
                    [&scal](float a) -> float {
                        return a * scal.getFloatVal()[0];
                    }
                    );
                break;
            }
            case BOOL_T:
                assert(0);
            default:
                assert(0);
        }

        return result;
    }
}

DataContainer operator/(const DataContainer &lhs, const DataContainer &rhs) {
    /*
     * /, ˆ ss Arithmetic
     */
    assert(lhs.getType() == rhs.getType());
    assert(lhs.getTypeOrder() == 1);
    assert(getDataTypeCategory(lhs.getType()) == DataTypeCategory::Arithmetic);

    DataContainer result(lhs.getType());
    switch(lhs.getTypeBase()) {
        case INT_T: {
            result.getIntVal()[0] = lhs.getIntVal()[0] / rhs.getIntVal()[0];
            break;
        }
        case FLOAT_T: {
            result.getFloatVal()[0] = lhs.getFloatVal()[0] / rhs.getFloatVal()[0];
            break;
        }
        case BOOL_T:
            assert(0);
        default:
            assert(0);
    }

    return result;
}

DataContainer operator^(const DataContainer &lhs, const DataContainer &rhs) {
    /*
     * /, ˆ ss Arithmetic
     */
    assert(lhs.getType() == rhs.getType());
    assert(lhs.getTypeOrder() == 1);
    assert(getDataTypeCategory(lhs.getType()) == DataTypeCategory::Arithmetic);

    DataContainer result(lhs.getType());
    switch(lhs.getTypeBase()) {
        case INT_T: {
            result.getIntVal()[0] = static_cast<int>(pow(lhs.getIntVal()[0], rhs.getIntVal()[0]));
            break;
        }
        case FLOAT_T: {
            result.getFloatVal()[0] = static_cast<float>(pow(lhs.getFloatVal()[0], rhs.getFloatVal()[0]));
            break;
        }
        case BOOL_T:
            assert(0);
        default:
            assert(0);
    }

    return result;
}

DataContainer operator&&(const DataContainer &lhs, const DataContainer &rhs) {
    /*
    * &&, || ss, vv Logical
     */
    assert(lhs.getType() == rhs.getType());
    assert(getDataTypeCategory(lhs.getType()) == DataTypeCategory::Boolean);

    DataContainer result(lhs.getType());
    switch(lhs.getTypeBase()) {     
        case BOOL_T: {
            std::transform(lhs.getBoolValBegin(), lhs.getBoolValEnd(), rhs.getBoolValBegin(), result.getBoolVal().begin(), 
                [](bool a, bool b) -> bool {
                    return a && b;
                }
                );
            break;
        }
        case INT_T:
            assert(0);
        case FLOAT_T:
            assert(0);
        default:
            assert(0);
    }

    return result;
}

DataContainer operator||(const DataContainer &lhs, const DataContainer &rhs) {
    /*
    * &&, || ss, vv Logical
     */
    assert(lhs.getType() == rhs.getType());
    assert(getDataTypeCategory(lhs.getType()) == DataTypeCategory::Boolean);

    DataContainer result(lhs.getType());
    switch(lhs.getTypeBase()) {     
        case BOOL_T: {
            std::transform(lhs.getBoolValBegin(), lhs.getBoolValEnd(), rhs.getBoolValBegin(), result.getBoolVal().begin(), 
                [](bool a, bool b) -> bool {
                    return a || b;
                }
                );
            break;
        }
        case INT_T:
            assert(0);
        case FLOAT_T:
            assert(0);
        default:
            assert(0);
    }

    return result;
}

DataContainer operator<(const DataContainer &lhs, const DataContainer &rhs) {
    /*
     * <, <=, >, >= ss Comparison
     */
    assert(lhs.getType() == rhs.getType());
    assert(lhs.getTypeOrder() == 1);
    assert(getDataTypeCategory(lhs.getType()) == DataTypeCategory::Arithmetic);

    DataContainer result(BOOL_T);
    switch(lhs.getTypeBase()) {
        case INT_T: {
            result.getBoolVal()[0] = lhs.getIntVal()[0] < rhs.getIntVal()[0];
            break;
        }
        case FLOAT_T: {
            result.getBoolVal()[0] = lhs.getFloatVal()[0] < rhs.getFloatVal()[0];
            break;
        }
        case BOOL_T:
            assert(0);
        default:
            assert(0);
    }

    return result;
}

DataContainer operator<=(const DataContainer &lhs, const DataContainer &rhs) {
    /*
     * <, <=, >, >= ss Comparison
     */
    assert(lhs.getType() == rhs.getType());
    assert(lhs.getTypeOrder() == 1);
    assert(getDataTypeCategory(lhs.getType()) == DataTypeCategory::Arithmetic);

    DataContainer result(BOOL_T);
    switch(lhs.getTypeBase()) {
        case INT_T: {
            result.getBoolVal()[0] = lhs.getIntVal()[0] <= rhs.getIntVal()[0];
            break;
        }
        case FLOAT_T: {
            result.getBoolVal()[0] = lhs.getFloatVal()[0] <= rhs.getFloatVal()[0];
            break;
        }
        case BOOL_T:
            assert(0);
        default:
            assert(0);
    }

    return result;
}

DataContainer operator>(const DataContainer &lhs, const DataContainer &rhs) {
    /*
     * <, <=, >, >= ss Comparison
     */
    assert(lhs.getType() == rhs.getType());
    assert(lhs.getTypeOrder() == 1);
    assert(getDataTypeCategory(lhs.getType()) == DataTypeCategory::Arithmetic);

    DataContainer result(BOOL_T);
    switch(lhs.getTypeBase()) {
        case INT_T: {
            result.getBoolVal()[0] = lhs.getIntVal()[0] > rhs.getIntVal()[0];
            break;
        }
        case FLOAT_T: {
            result.getBoolVal()[0] = lhs.getFloatVal()[0] > rhs.getFloatVal()[0];
            break;
        }
        case BOOL_T:
            assert(0);
        default:
            assert(0);
    }

    return result;
}

DataContainer operator>=(const DataContainer &lhs, const DataContainer &rhs) {
    /*
     * <, <=, >, >= ss Comparison
     */
    assert(lhs.getType() == rhs.getType());
    assert(lhs.getTypeOrder() == 1);
    assert(getDataTypeCategory(lhs.getType()) == DataTypeCategory::Arithmetic);

    DataContainer result(BOOL_T);
    switch(lhs.getTypeBase()) {
        case INT_T: {
            result.getBoolVal()[0] = lhs.getIntVal()[0] >= rhs.getIntVal()[0];
            break;
        }
        case FLOAT_T: {
            result.getBoolVal()[0] = lhs.getFloatVal()[0] >= rhs.getFloatVal()[0];
            break;
        }
        case BOOL_T:
            assert(0);
        default:
            assert(0);
    }

    return result;
}

DataContainer operator==(const DataContainer &lhs, const DataContainer &rhs) {
    /*
     * ==, != ss, vv Comparison
     */
    assert(lhs.getType() == rhs.getType());
    assert(getDataTypeCategory(lhs.getType()) == DataTypeCategory::Arithmetic);

    DataContainer result(BOOL_T);
    switch(lhs.getTypeBase()) {
        case INT_T: {
            result.getBoolVal()[0] = std::equal(lhs.getIntValBegin(), lhs.getIntValEnd(), rhs.getIntValBegin());
            break;
        }
        case FLOAT_T: {
            result.getBoolVal()[0] = std::equal(lhs.getFloatValBegin(), lhs.getFloatValEnd(), rhs.getFloatValBegin());
            break;
        }
        case BOOL_T:
            assert(0);
        default:
            assert(0);
    }

    return result;
}

DataContainer operator!=(const DataContainer &lhs, const DataContainer &rhs) {
    /*
     * ==, != ss, vv Comparison
     */
    assert(lhs.getType() == rhs.getType());
    assert(getDataTypeCategory(lhs.getType()) == DataTypeCategory::Arithmetic);

    DataContainer result(BOOL_T);
    switch(lhs.getTypeBase()) {
        case INT_T: {
            result.getBoolVal()[0] = !std::equal(lhs.getIntValBegin(), lhs.getIntValEnd(), rhs.getIntValBegin());
            break;
        }
        case FLOAT_T: {
            result.getBoolVal()[0] = !std::equal(lhs.getFloatValBegin(), lhs.getFloatValEnd(), rhs.getFloatValBegin());
            break;
        }
        case BOOL_T:
            assert(0);
        default:
            assert(0);
    }

    return result;
}

DataContainer operator-(const DataContainer &rhs) {
    /*
     * - s, v Arithmetic
     */
    assert(getDataTypeCategory(rhs.getType()) == DataTypeCategory::Arithmetic);

    DataContainer result(rhs.getType());
    switch(rhs.getTypeBase()) {
        case INT_T: {
            std::transform(rhs.getIntValBegin(), rhs.getIntValEnd(), result.getIntVal().begin(), 
                [](int a) -> int {
                    return -a;
                }
                );
            break;
        }
        case FLOAT_T: {
            std::transform(rhs.getFloatValBegin(), rhs.getFloatValEnd(), result.getFloatVal().begin(), 
                [](float a) -> float {
                    return -a;
                }
                );
            break;
        }
        case BOOL_T:
            assert(0);
        default:
            assert(0);
    }

    return result;
}

DataContainer operator!(const DataContainer &rhs) {
    /*
     * ! s, v Logical
     */
    assert(getDataTypeCategory(rhs.getType()) == DataTypeCategory::Boolean);

    DataContainer result(rhs.getType());
    switch(rhs.getTypeBase()) {
        case BOOL_T: {
            std::transform(rhs.getBoolValBegin(), rhs.getBoolValEnd(), result.getBoolVal().begin(), 
                [](bool a) -> bool {
                    return !a;
                }
                );
            break;
        }
        case INT_T:
            assert(0);
        case FLOAT_T:
            assert(0);
        default:
            assert(0);
    }

    return result;
}

DataContainer& DataContainer::operator= (const DataContainer &rhs) {
    if(this == &rhs) {
        return *this;
    }

    assert((this->getType() == rhs.getType()));
    this->m_value = rhs.m_value;

    return *this;
}

DataContainer DataContainer::getSlice(int idx) {
    assert(idx >= 0);
    assert(idx < this->m_typeOrder);
    DataContainer result(this->m_typeBase);
    switch(this->m_typeBase) {
        case INT_T: {
            result.getIntVal()[0] = this->getIntVal()[idx];
            break;
        }
        case FLOAT_T: {
            result.getFloatVal()[0] = this->getFloatVal()[idx];
            break;
        }
        case BOOL_T: {
            result.getBoolVal()[0] = this->getBoolVal()[idx];
            break;
        }
        default:
            assert(0);
    }

    return result;
}

AST::ExpressionNode *DataContainer::createASTExpr() const {
    AST::ExpressionNode *resultExpr = nullptr;

    switch(m_typeBase) {
        case INT_T: {
            const std::array<int, 4> &val = getIntVal();
            if(m_typeOrder > 1) {
                AST::ExpressionsNode *args = new AST::ExpressionsNode();
                    for(int i = 0; i < m_typeOrder; i++) {       
                        args->pushBackExpression(new AST::IntLiteralNode(val[i]));
                    }
                AST::ConstructorNode *constructor = new AST::ConstructorNode(m_type, args);
                resultExpr = constructor;
            } else {
                AST::IntLiteralNode *intLit = new AST::IntLiteralNode(val[0]);
                resultExpr = intLit;
            }

            break;
        }

        case FLOAT_T: {
            const std::array<float, 4> &val = getFloatVal();
            if(m_typeOrder > 1) {
                AST::ExpressionsNode *args = new AST::ExpressionsNode();
                    for(int i = 0; i < m_typeOrder; i++) {       
                        args->pushBackExpression(new AST::FloatLiteralNode(val[i]));
                    }
                AST::ConstructorNode *constructor = new AST::ConstructorNode(m_type, args);
                resultExpr = constructor;
            } else {
                AST::FloatLiteralNode *floatLit = new AST::FloatLiteralNode(val[0]);
                resultExpr = floatLit;
            }

            break;
        }

        case BOOL_T: {
            const std::array<bool, 4> &val = getBoolVal();
            if(m_typeOrder > 1) {
                AST::ExpressionsNode *args = new AST::ExpressionsNode();
                    for(int i = 0; i < m_typeOrder; i++) {       
                        args->pushBackExpression(new AST::BooleanLiteralNode(val[i]));
                    }
                AST::ConstructorNode *constructor = new AST::ConstructorNode(m_type, args);
                resultExpr = constructor;
            } else {
                AST::BooleanLiteralNode *boolLit = new AST::BooleanLiteralNode(val[0]);
                resultExpr = boolLit;
            }

            break;
        }

        default:
            assert(0);
    }

    if(resultExpr != nullptr) {
        resultExpr->setConst(true);
        resultExpr->setExpressionType(m_type);
    }

    return resultExpr;
}

class ConstantExpressionEvaluator: public AST::Visitor {
    private:
        bool m_evaluationSuccessful = true;
        DataContainer &m_data;

    private:
        ConstantExpressionEvaluator(DataContainer &data): m_data(data) {}

    private:
        virtual void nodeVisit(AST::UnaryExpressionNode *unaryExpressionNode) {
            assert(m_data.getType() == unaryExpressionNode->getExpressionType());

            AST::ExpressionNode *rhsExpr = unaryExpressionNode->getExpression();
            DataContainer rhsData(rhsExpr->getExpressionType());
            m_evaluationSuccessful = evaluateValue(rhsExpr, rhsData);
            
            if(m_evaluationSuccessful) {
                switch(unaryExpressionNode->getOperator()) {
                    case MINUS:
                        m_data = -rhsData;
                        break;
                    case NOT:
                        m_data = !rhsData;
                        break;
                    default:
                        assert(0);
                }
            }
        }
        
        virtual void nodeVisit(AST::BinaryExpressionNode *binaryExpressionNode) {
            assert(m_data.getType() == binaryExpressionNode->getExpressionType());

            AST::ExpressionNode *lhsExpr = binaryExpressionNode->getLeftExpression();
            DataContainer lhsData(lhsExpr->getExpressionType());
            m_evaluationSuccessful = evaluateValue(lhsExpr, lhsData);

            AST::ExpressionNode *rhsExpr = binaryExpressionNode->getRightExpression();
            DataContainer rhsData(rhsExpr->getExpressionType());
            m_evaluationSuccessful &= evaluateValue(rhsExpr, rhsData);

            if(m_evaluationSuccessful) {
                switch(binaryExpressionNode->getOperator()) {
                    case AND:
                        m_data = lhsData && rhsData;
                        break;
                    case OR:
                        m_data = lhsData || rhsData;
                        break;
                    case PLUS:
                        m_data = lhsData + rhsData;
                        break;
                    case MINUS:
                        m_data = lhsData - rhsData;
                        break;
                    case TIMES:
                        m_data = lhsData * rhsData;
                        break;
                    case SLASH:
                        m_data = lhsData / rhsData;
                        break;
                    case EXP:
                        m_data = lhsData ^ rhsData;
                        break;
                    case EQL:
                        m_data = lhsData == rhsData;
                        break;
                    case NEQ:
                        m_data = lhsData != rhsData;
                        break;
                    case LSS:
                        m_data = lhsData < rhsData;
                        break;
                    case LEQ:
                        m_data = lhsData <= rhsData;
                        break;
                    case GTR:
                        m_data = lhsData > rhsData;
                        break;
                    case GEQ:
                        m_data = lhsData >= rhsData;
                        break;
                    default:
                        assert(0);
                }
            }
        }

        virtual void nodeVisit(AST::IntLiteralNode *intLiteralNode) {
            assert(m_data.getType() == INT_T);

            m_data.getIntVal()[0] = intLiteralNode->getVal();
        }

        virtual void nodeVisit(AST::FloatLiteralNode *floatLiteralNode) {
            assert(m_data.getType() == FLOAT_T);

            m_data.getFloatVal()[0] = floatLiteralNode->getVal();
        }

        virtual void nodeVisit(AST::BooleanLiteralNode *booleanLiteralNode) {
            assert(m_data.getType() == BOOL_T);

            m_data.getBoolVal()[0] = booleanLiteralNode->getVal();
        }

        virtual void nodeVisit(AST::IdentifierNode *identifierNode) {
            if(!identifierNode->isOrdinaryType()) {
                m_evaluationSuccessful = false;
                return;
            }
            assert(m_data.getType() == identifierNode->getExpressionType());

            const AST::DeclarationNode *decl = identifierNode->getDeclaration();
            assert(decl != nullptr);

            AST::ExpressionNode *identVal = decl->getInitValue();
            if(identVal == nullptr) {
                /* If somehow this variable does not have a evaluated initialization expression, abort */
                m_evaluationSuccessful = false;
                return;
            }

            m_evaluationSuccessful = evaluateValue(identVal, m_data);
        }

        virtual void nodeVisit(AST::IndexingNode *indexingNode) {
            if(!indexingNode->isOrdinaryType()) {
                m_evaluationSuccessful = false;
                return;
            }
            assert(m_data.getType() == indexingNode->getExpressionType());

            AST::IdentifierNode *ident = indexingNode->getIdentifier();
            int identType = ident->getExpressionType();
            assert(getDataTypeBaseType(identType) == indexingNode->getExpressionType());

            /* Get the value of identifier first */
            DataContainer identData(identType);
            m_evaluationSuccessful = evaluateValue(ident, identData);

            if(m_evaluationSuccessful) {
                /* Do the indexing */
                AST::IntLiteralNode *index = reinterpret_cast<AST::IntLiteralNode *>(indexingNode->getIndexExpression());
                m_data = identData.getSlice(index->getVal());
            }
        }

        virtual void nodeVisit(AST::FunctionNode *functionNode) {
            m_evaluationSuccessful = false;
        }

        virtual void nodeVisit(AST::ConstructorNode *constructorNode) {
            assert(m_data.getType() == constructorNode->getExpressionType());

            const AST::ExpressionsNode *exprs = constructorNode->getArgumentExpressions();
            const std::vector<AST::ExpressionNode *> &args = exprs->getExpressionList();

            int constructorDataType = constructorNode->getConstructorType();
            int constructorDataTypeBaseType = getDataTypeBaseType(constructorDataType);
            for(int i = 0; i < args.size() && m_evaluationSuccessful; i++) {
                assert(args[i]->getExpressionType() == constructorDataTypeBaseType);
                assert(i < m_data.getTypeOrder());
                
                DataContainer argData(constructorDataTypeBaseType);
                m_evaluationSuccessful &= evaluateValue(args[i], argData);

                switch(constructorDataTypeBaseType) {
                    case INT_T:
                        m_data.getIntVal()[i] = argData.getIntVal()[0];
                        break;
                    case FLOAT_T:
                        m_data.getFloatVal()[i] = argData.getFloatVal()[0];
                        break;
                    case BOOL_T:
                        m_data.getBoolVal()[i] = argData.getBoolVal()[0];
                        break;
                    default:
                        assert(0);
                }
            }
        }

    public:
        static bool evaluateValue(AST::ExpressionNode *expr, DataContainer &data) {
            int dataType = expr->getExpressionType();
            assert(dataType != ANY_TYPE);
            assert(dataType == data.getType());

            ConstantExpressionEvaluator ev(data);
            expr->visit(ev);

            return ev.m_evaluationSuccessful;
        }
};

class ConstantDeclarationOptimizer: public AST::Visitor {
    private:
        virtual void preNodeVisit(AST::DeclarationNode *declarationNode);

    private:
        /* Do not traverse into these nodes */
        virtual void nodeVisit(AST::DeclarationNode *declarationNode) {}

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
};

void ConstantDeclarationOptimizer::preNodeVisit(AST::DeclarationNode *declarationNode) {
    /* Skip fake declarations for predefined variables */
    if(!declarationNode->isOrdinaryType()) {
        return;
    }

    /* Skip if declaration is not for a const-qualified variable */
    if(!declarationNode->isConst()) {
        return;
    }

    AST::ExpressionNode *initExpr = declarationNode->getExpression();
    
    /* Skip if declaration does not have initialization */
    if(initExpr == nullptr) {
        return;
    }

    /* Skip if initialization is not well-defined */
    if(initExpr->getExpressionType() == ANY_TYPE) {
        return;
    }

    /* Skip if initialization is not a const-qualifed expression */
    if(!initExpr->isConst()) {
        return;
    }

    /* Skip if type does not match */
    if(declarationNode->getType() != initExpr->getExpressionType()) {
        return;
    }

    /* 
     * We have skipped a lot of invalid conditions that are not suitable for
     * constant folding and propagation, even some combinations of them
     * are not valid in terms of the lanague grammar rule.
     * However, it is okay to simply skip them, because these errors are caught
     * in other stages of semantic analysis.
     */
    DataContainer initData(initExpr->getExpressionType());
    bool constOptSuccessful = ConstantExpressionEvaluator::evaluateValue(initExpr, initData);
    if(constOptSuccessful) {
        fprintf(outputFile, "Info: Optimization for declaration of const-qualified symbol '%s' of type '%s%s' successful at %s.\n",
            declarationNode->getName().c_str(), declarationNode->getQualifierString().c_str(), declarationNode->getTypeString().c_str(), 
            declarationNode->getSourceLocationString().c_str());

        AST::ExpressionNode *resultExpr = initData.createASTExpr();
        assert(resultExpr != nullptr);
        declarationNode->setInitValue(resultExpr);
    }
}

class VariableAssignmentLog {
    private:
        VariableAssignmentLog *m_prev;
        std::unordered_set<const AST::DeclarationNode *> m_assignmentLU;

    public:
        VariableAssignmentLog(VariableAssignmentLog *prev) : m_prev(prev) {}

    public:
        void markAssigned(const AST::DeclarationNode *decl);
        bool checkAssigned(const AST::DeclarationNode *decl) const; // Checks recursively
        void merge(const VariableAssignmentLog &log1, const VariableAssignmentLog &log2);
        void merge(const VariableAssignmentLog &log);
};

void VariableAssignmentLog::markAssigned(const AST::DeclarationNode *decl) {
    assert(decl);
    m_assignmentLU.insert(decl);
}

bool VariableAssignmentLog::checkAssigned(const AST::DeclarationNode *decl) const {
    assert(decl);

    // Checks recursively
    if(m_assignmentLU.count(decl) == 1) {
        // If found, return immediately
        return true;
    }

    // If not found, check previous node
    if(m_prev != nullptr) {
        return m_prev->checkAssigned(decl);
    } else {
        // If not found and reaches the end
        return false;
    }
}

void VariableAssignmentLog::merge(const VariableAssignmentLog &log1, const VariableAssignmentLog &log2) {
    std::unordered_set<const AST::DeclarationNode *> intersection;
    for(const AST::DeclarationNode *node: log1.m_assignmentLU) {
        if(log2.m_assignmentLU.count(node) == 1) {
            intersection.insert(node);
        }
    }

    this->m_assignmentLU.insert(intersection.begin(), intersection.end());
}

void VariableAssignmentLog::merge(const VariableAssignmentLog &log) {
    this->m_assignmentLU.insert(log.m_assignmentLU.begin(), log.m_assignmentLU.end());
}

class VariableAssignmentChecker: public AST::Visitor {
    private:
        SemanticAnalyzer &m_semanticAnalyzer;

    private:
        VariableAssignmentLog *m_currentFlowEdge = nullptr;

        const AST::DeclarationNode *m_currentDeclaration = nullptr;
        bool m_currentDeclarationRecursiveInit = false;

    public:
        VariableAssignmentChecker(SemanticAnalyzer &semanticAnalyzer):
            m_semanticAnalyzer(semanticAnalyzer) {}

    private:
        virtual void preNodeVisit(AST::DeclarationNode *declarationNode);

    private:
        virtual void postNodeVisit(AST::AssignmentNode *assignmentNode);
        virtual void postNodeVisit(AST::IdentifierNode *identifierNode);
        virtual void postNodeVisit(AST::DeclarationNode *declarationNode);

    private:
        virtual void nodeVisit(AST::AssignmentNode *assignmentNode);
        virtual void nodeVisit(AST::IfStatementNode *ifStatementNode);
        virtual void nodeVisit(AST::ScopeNode *scopeNode);
};

void VariableAssignmentChecker::preNodeVisit(AST::DeclarationNode *declarationNode) {
    assert(m_currentDeclaration == nullptr);
    m_currentDeclaration = declarationNode;
    m_currentDeclarationRecursiveInit = false;
}

void VariableAssignmentChecker::postNodeVisit(AST::AssignmentNode *assignmentNode) {
    if(assignmentNode->getExpressionType() == ANY_TYPE) {
        return;
    }

    AST::VariableNode *var = assignmentNode->getVariable();
    if(!var->isOrdinaryType()) {
        // Skip for predefined variables
        return;
    }

    const AST::DeclarationNode *decl = var->getDeclaration();
    assert(decl != nullptr);

    m_currentFlowEdge->markAssigned(decl);
}

void VariableAssignmentChecker::postNodeVisit(AST::IdentifierNode *identifierNode) {
    if(!identifierNode->isOrdinaryType()) {
        // Skip for predefined variables
        return;
    }

    if(identifierNode->getExpressionType() == ANY_TYPE) {
        return;
    }

    const AST::DeclarationNode *decl = identifierNode->getDeclaration();
    assert(decl != nullptr);

    if(!m_currentFlowEdge->checkAssigned(decl)) {
        auto id = m_semanticAnalyzer.createEvent(identifierNode, SemanticAnalyzer::EventType::Warning);
        
        std::stringstream ss;
        ss << "Read of potentially unassigned variable '" << identifierNode->getName() << "' of type '" <<
            identifierNode->getExpressionQualifierString() << identifierNode->getExpressionTypeString() << "' at " <<
            identifierNode->getSourceLocationString() << ".";

        m_semanticAnalyzer.getEvent(id).Message() = std::move(ss.str());
        m_semanticAnalyzer.getEvent(id).EventLoc() = identifierNode->getSourceLocation();

        if(decl == m_currentDeclaration) {
            /* use of a variable within its own initialization */
            m_currentDeclarationRecursiveInit = true;
        }
    }
}

void VariableAssignmentChecker::postNodeVisit(AST::DeclarationNode *declarationNode) {
    assert(m_currentDeclaration == declarationNode);
    m_currentDeclaration = nullptr;

    AST::ExpressionNode *initExpr = declarationNode->getExpression();
    if(initExpr == nullptr) {
        // Declaration without initialization
        return;
    }

    if(initExpr->getExpressionType() != declarationNode->getType()) {
        return;
    }

    if(m_currentDeclarationRecursiveInit) {
        auto id = m_semanticAnalyzer.createEvent(declarationNode, SemanticAnalyzer::EventType::Warning);
        
        std::stringstream ss;
        ss << "Self-initialization involves read of potentially unassigned variable '" << declarationNode->getName() << "' of type '" <<
            declarationNode->getQualifierString() << declarationNode->getTypeString() << "' at " <<
            declarationNode->getSourceLocationString() << ".";

        m_semanticAnalyzer.getEvent(id).Message() = std::move(ss.str());
        m_semanticAnalyzer.getEvent(id).EventLoc() = declarationNode->getSourceLocation();

        m_currentDeclarationRecursiveInit = false;
    } else {
        m_currentFlowEdge->markAssigned(declarationNode);
    }
}

void VariableAssignmentChecker::nodeVisit(AST::AssignmentNode *assignmentNode) {
    // Do not traverse into variable on lhs
    assignmentNode->getExpression()->visit(*this);
}

void VariableAssignmentChecker::nodeVisit(AST::IfStatementNode *ifStatementNode) {
    AST::ExpressionNode *cond = ifStatementNode->getConditionExpression();
    cond->visit(*this);

    VariableAssignmentLog *commonParentEdge = m_currentFlowEdge;

    // Branch
    std::unique_ptr<VariableAssignmentLog> thenStmtEdge(new VariableAssignmentLog(commonParentEdge));
    m_currentFlowEdge = thenStmtEdge.get();
    ifStatementNode->getThenStatement()->visit(*this);

    // Branch
    std::unique_ptr<VariableAssignmentLog> elseStmtEdge(new VariableAssignmentLog(commonParentEdge));
    m_currentFlowEdge = elseStmtEdge.get();
    if(ifStatementNode->getElseStatement() != nullptr) {
        ifStatementNode->getElseStatement()->visit(*this);
    }

    // If there is a always true branch
    VariableAssignmentLog *alwaysTrueBranch = nullptr;
    if(cond->isConst() && cond->getExpressionType() == BOOL_T) {
        DataContainer condData(BOOL_T);
        bool successful = ConstantExpressionEvaluator::evaluateValue(cond, condData);

        if(successful) {
            alwaysTrueBranch = condData.getBoolVal()[0] ? thenStmtEdge.get() : elseStmtEdge.get();
        }
    }

    // Merge
    if(alwaysTrueBranch == nullptr) {
        commonParentEdge->merge(*thenStmtEdge, *elseStmtEdge);
    } else {
        commonParentEdge->merge(*alwaysTrueBranch);
    }

    m_currentFlowEdge = commonParentEdge;
}

void VariableAssignmentChecker::nodeVisit(AST::ScopeNode *scopeNode) {
    std::unique_ptr<VariableAssignmentLog> rootEdge(new VariableAssignmentLog(nullptr));
    m_currentFlowEdge = rootEdge.get();

    scopeNode->getDeclarations()->visit(*this);
    scopeNode->getStatements()->visit(*this);

    m_currentFlowEdge = nullptr;
}

} /* END NAMESPACE */

int semantic_check(node * ast) {
    ST::SymbolTable symbolTable;
    SEMA::SemanticAnalyzer semaAnalyzer;
    SEMA::SourceContext sourceContext(inputFile);

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
    // printf("***************************************\n");
    // printf("AST DUMP POST TYPE CHECK\n");
    // ast_print(ast);

    /* Evaluate initialization for const-qualified declaration */
    SEMA::ConstantDeclarationOptimizer constDeclOptimizer;
    static_cast<AST::ASTNode *>(ast)->visit(constDeclOptimizer);
    // printf("***************************************\n");
    // printf("AST DUMP POST CONST DECL OPT\n");
    // ast_print(ast);

    /* Ensure that every variable has been assigned a value before being read */
    SEMA::VariableAssignmentChecker varAssignmentChecker(semaAnalyzer);
    static_cast<AST::ASTNode *>(ast)->visit(varAssignmentChecker);

    /* Check Semantic Analysis Result */
    int numEvents = semaAnalyzer.getNumberEvents();
    #ifdef NORMAL_MODE
    semaAnalyzer.setColorPrintEnabled(true);
    semaAnalyzer.setOutput(stdout);
    #else
    semaAnalyzer.setColorPrintEnabled(false);
    semaAnalyzer.setOutput(errorFile);
    #endif
    if(numEvents != 0) {
        fprintf(semaAnalyzer.getOutput(), "\n");
    }
    for(int id = 0; id < numEvents; id++) {
        semaAnalyzer.printEvent(id, sourceContext);
    }
    if(numEvents != 0) {
        fprintf(semaAnalyzer.getOutput(), "--------------------------------------------------------------------------\n");
    }
    
    int numErrorEvents = semaAnalyzer.getNumberErrors();
    if(numErrorEvents > 0) {
        // If error occurs, return 0
        errorOccurred = 1;
        return 0;
    } else {
        // if no error, return 1
        return 1;
    }
}