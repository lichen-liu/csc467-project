#ifndef AST_H_
#define AST_H_ 1

#include <stdarg.h>

#include <string>
#include <vector>
#include <list>

//////////////////////////////////////////////////////////////////
//
// A Modern Object-Oriented Approach for AST
//
//////////////////////////////////////////////////////////////////
#define ANY_TYPE -1

namespace AST{

class ExpressionNode;
class ExpressionsNode;
class UnaryExpressionNode;
class BinaryExpressionNode;
class IntLiteralNode;
class FloatLiteralNode;
class BooleanLiteralNode;
class VariableNode;
class IdentifierNode;
class IndexingNode;
class FunctionNode;
class ConstructorNode;
class StatementNode;
class StatementsNode;
class DeclarationNode;
class DeclarationsNode;
class IfStatementNode;
class WhileStatementNode;
class AssignmentNode;
class StallStatementNode;
class NestedScopeNode;
class ScopeNode;

class Visitor {
    protected:
        Visitor() = default;

    private:
        /* Post-Node Visit */
        virtual void postNodeVisit(ExpressionNode *expressionNode){}
        virtual void postNodeVisit(ExpressionsNode *expressionsNode){}
        virtual void postNodeVisit(UnaryExpressionNode *unaryExpressionNode){}
        virtual void postNodeVisit(BinaryExpressionNode *binaryExpressionNode){}
        virtual void postNodeVisit(IntLiteralNode *intLiteralNode){}
        virtual void postNodeVisit(FloatLiteralNode *floatLiteralNode){}
        virtual void postNodeVisit(BooleanLiteralNode *booleanLiteralNode){}
        virtual void postNodeVisit(VariableNode *variableNode){}
        virtual void postNodeVisit(IdentifierNode *identifierNode){}
        virtual void postNodeVisit(IndexingNode *indexingNode){}
        virtual void postNodeVisit(FunctionNode *functionNode){}
        virtual void postNodeVisit(ConstructorNode *constructorNode){}
        virtual void postNodeVisit(StatementNode *statementNode){}
        virtual void postNodeVisit(StatementsNode *statementsNode){}
        virtual void postNodeVisit(DeclarationNode *declarationNode){}
        virtual void postNodeVisit(DeclarationsNode *declarationsNode){}
        virtual void postNodeVisit(IfStatementNode *ifStatementNode){}
        virtual void postNodeVisit(WhileStatementNode *whileStatementNode){}
        virtual void postNodeVisit(AssignmentNode *assignmentNode){}
        virtual void postNodeVisit(StallStatementNode *stallStatementNode){}
        virtual void postNodeVisit(NestedScopeNode *nestedScopeNode){}
        virtual void postNodeVisit(ScopeNode *scopeNode){}

    private:
        /* Pre-Node Visit */
        virtual void preNodeVisit(ExpressionNode *expressionNode){}
        virtual void preNodeVisit(ExpressionsNode *expressionsNode){}
        virtual void preNodeVisit(UnaryExpressionNode *unaryExpressionNode){}
        virtual void preNodeVisit(BinaryExpressionNode *binaryExpressionNode){}
        virtual void preNodeVisit(IntLiteralNode *intLiteralNode){}
        virtual void preNodeVisit(FloatLiteralNode *floatLiteralNode){}
        virtual void preNodeVisit(BooleanLiteralNode *booleanLiteralNode){}
        virtual void preNodeVisit(VariableNode *variableNode){}
        virtual void preNodeVisit(IdentifierNode *identifierNode){}
        virtual void preNodeVisit(IndexingNode *indexingNode){}
        virtual void preNodeVisit(FunctionNode *functionNode){}
        virtual void preNodeVisit(ConstructorNode *constructorNode){}
        virtual void preNodeVisit(StatementNode *statementNode){}
        virtual void preNodeVisit(StatementsNode *statementsNode){}
        virtual void preNodeVisit(DeclarationNode *declarationNode){}
        virtual void preNodeVisit(DeclarationsNode *declarationsNode){}
        virtual void preNodeVisit(IfStatementNode *ifStatementNode){}
        virtual void preNodeVisit(WhileStatementNode *whileStatementNode){}
        virtual void preNodeVisit(AssignmentNode *assignmentNode){}
        virtual void preNodeVisit(StallStatementNode *stallStatementNode){}
        virtual void preNodeVisit(NestedScopeNode *nestedScopeNode){}
        virtual void preNodeVisit(ScopeNode *scopeNode){}

    private:
        /* Default Implementation for Node -> Sub-Node Traversal */
        virtual void nodeVisit(ExpressionNode *expressionNode);
        virtual void nodeVisit(ExpressionsNode *expressionsNode);
        virtual void nodeVisit(UnaryExpressionNode *unaryExpressionNode);
        virtual void nodeVisit(BinaryExpressionNode *binaryExpressionNode);
        virtual void nodeVisit(IntLiteralNode *intLiteralNode);
        virtual void nodeVisit(FloatLiteralNode *floatLiteralNode);
        virtual void nodeVisit(BooleanLiteralNode *booleanLiteralNode);
        virtual void nodeVisit(VariableNode *variableNode);
        virtual void nodeVisit(IdentifierNode *identifierNode);
        virtual void nodeVisit(IndexingNode *indexingNode);
        virtual void nodeVisit(FunctionNode *functionNode);
        virtual void nodeVisit(ConstructorNode *constructorNode);
        virtual void nodeVisit(StatementNode *statementNode);
        virtual void nodeVisit(StatementsNode *statementsNode);
        virtual void nodeVisit(DeclarationNode *declarationNode);
        virtual void nodeVisit(DeclarationsNode *declarationsNode);
        virtual void nodeVisit(IfStatementNode *ifStatementNode);
        virtual void nodeVisit(WhileStatementNode *whileStatementNode);
        virtual void nodeVisit(AssignmentNode *assignmentNode);
        virtual void nodeVisit(StallStatementNode *stallStatementNode);
        virtual void nodeVisit(NestedScopeNode *nestedScopeNode);
        virtual void nodeVisit(ScopeNode *scopeNode);

    public:
        /* Node Traversal Framework */
        void visit(ExpressionNode *);
        void visit(ExpressionsNode *);
        void visit(UnaryExpressionNode *);
        void visit(BinaryExpressionNode *);
        void visit(IntLiteralNode *);
        void visit(FloatLiteralNode *);
        void visit(BooleanLiteralNode *);
        void visit(VariableNode *);
        void visit(IdentifierNode *);
        void visit(IndexingNode *);
        void visit(FunctionNode *);
        void visit(ConstructorNode *);
        void visit(StatementNode *);
        void visit(StatementsNode *);
        void visit(DeclarationNode *);
        void visit(DeclarationsNode *);
        void visit(IfStatementNode *);
        void visit(WhileStatementNode *);
        void visit(AssignmentNode *);
        void visit(StallStatementNode *);
        void visit(NestedScopeNode *);
        void visit(ScopeNode *);
};

struct SourceLocation {
    int firstLine = 0;
    int firstColumn = 0;
    int lastLine = 0;
    int lastColumn = 0;

    friend bool operator==(const SourceLocation& lhs, const SourceLocation& rhs) {
        return (lhs.firstLine == rhs.firstLine) &&
               (lhs.firstColumn == rhs.firstColumn) &&
               (lhs.lastLine == rhs.lastLine) &&
               (lhs.lastColumn == rhs.lastColumn);
    }
    friend bool operator!=(const SourceLocation& lhs, const SourceLocation& rhs){ return !(lhs == rhs); }
};

std::string getTypeString(int type);
std::string getOperatorString(int op);
std::string getSourceLocationString(const SourceLocation &srcLoc);

#define AST_VISIT_THIS_NODE     public:                                             \
                                virtual void visit(Visitor &visitor) {              \
                                    visitor.visit(this);                            \
                                }

/*
 * Base class for AST Nodes
 */
class ASTNode {
    private:
        SourceLocation m_srcLoc = {0};                  // source location for text correspondance of an AST node
    public:
        virtual void visit(Visitor &vistor) = 0;
    public:
        const SourceLocation &getSourceLocation() const { return m_srcLoc; }
        void setSourceLocation(const SourceLocation &srcLoc) { m_srcLoc = srcLoc; }
        std::string getSourceLocationString() const { return AST::getSourceLocationString(m_srcLoc); }
    protected:
        virtual ~ASTNode() {}
    public:
        /* The only way to destruct any AST ASTNode */
        static void destructNode(ASTNode *astNode) { delete astNode; }
};

class ExpressionNode: public ASTNode {
    /* Pure Virtual Intermediate Layer */
    public:
        virtual int getExpressionType() const = 0;      // pure virtual
        virtual void setExpressionType(int type) {}     // provide default definition
        virtual bool isConst() const = 0;               // pure virtual
        virtual void setConst(bool isConst) {}          // provide default definition
    public:
        std::string getExpressionTypeString() const { return getTypeString(getExpressionType()); }
        std::string getExpressionQualifierString() const { return isConst() ? "const " : ""; }
    protected:
        virtual ~ExpressionNode() {}
};

class ExpressionsNode: public ASTNode {
    private:
        std::vector<ExpressionNode *> m_expressions;    // A list of ExpressionNodes
    public:
        void pushBackExpression(ExpressionNode *expr) { m_expressions.push_back(expr); }
        const std::vector<ExpressionNode *> &getExpressionList() const { return m_expressions; }
    protected:
        virtual ~ExpressionsNode() {
            for(ExpressionNode *expr: m_expressions) {
                ASTNode::destructNode(expr);
            }
        }

    AST_VISIT_THIS_NODE
};

class UnaryExpressionNode: public ExpressionNode {
    private:
        int m_type = ANY_TYPE;                          // types defined in parser.tab.h
        bool m_isConst = false;                         // whether expression is const
        int m_op;                                       // unary operators defined in parser.tab.h
        ExpressionNode *m_expr;                         // sub-expression
    public:
        UnaryExpressionNode(int op, ExpressionNode *expr):
            m_op(op), m_expr(expr) {}
    public:
        virtual int getExpressionType() const { return m_type; }
        virtual void setExpressionType(int type) { m_type = type; }
        virtual bool isConst() const { return m_isConst; }
        virtual void setConst(bool isConst) { m_isConst = isConst; }
    public:
        int getOperator() const { return m_op; }
        ExpressionNode *getExpression() const { return m_expr; }
    protected:
        virtual ~UnaryExpressionNode() {
            ASTNode::destructNode(m_expr);
        }
    
    AST_VISIT_THIS_NODE
};

class BinaryExpressionNode: public ExpressionNode {
    private:
        int m_type = ANY_TYPE;                          // types defined in parser.tab.h
        bool m_isConst = false;                         // whether expression is const
        int m_op;                                       // binary operators defined in parser.tab.h
        ExpressionNode *m_leftExpr;                     // left sub-expression
        ExpressionNode *m_rightExpr;                    // right sub-expression
    public:
        BinaryExpressionNode(int op, ExpressionNode *leftExpr, ExpressionNode *rightExpr):
            m_op(op), m_leftExpr(leftExpr), m_rightExpr(rightExpr) {}
    public:
        virtual int getExpressionType() const { return m_type; }
        virtual void setExpressionType(int type) { m_type = type; }
        virtual bool isConst() const { return m_isConst; }
        virtual void setConst(bool isConst) { m_isConst = isConst; }
    public:
        int getOperator() const { return m_op; }
        ExpressionNode *getLeftExpression() const { return m_leftExpr; }
        ExpressionNode *getRightExpression() const { return m_rightExpr; }
    protected:
        virtual ~BinaryExpressionNode() {
            ASTNode::destructNode(m_leftExpr);
            ASTNode::destructNode(m_rightExpr);
        }
    
    AST_VISIT_THIS_NODE
};

class IntLiteralNode: public ExpressionNode {
    private:
        int m_val;                                      // value of this int literal
    public:
        IntLiteralNode(int val):
            m_val(val) {}
    public:
        virtual int getExpressionType() const;
        virtual bool isConst() const { return true; }
    public:
        int getVal() const { return m_val; }
    protected:
        virtual ~IntLiteralNode() {}
    
    AST_VISIT_THIS_NODE
};

class FloatLiteralNode: public ExpressionNode {
    private:
        float m_val;                                    // value of this float literal
    public:
        FloatLiteralNode(float val):
            m_val(val) {}
    public:
        virtual int getExpressionType() const;
        virtual bool isConst() const { return true; }
    public:
        float getVal() const { return m_val; }
    protected:
        virtual ~FloatLiteralNode() {}

    AST_VISIT_THIS_NODE
};

class BooleanLiteralNode: public ExpressionNode {
    private:
        bool m_val;                                     // value of this boolean literal
    public:
        BooleanLiteralNode(bool val):
            m_val(val) {}
    public:
        virtual int getExpressionType() const;
        virtual bool isConst() const { return true; }
    public:
        bool getVal() const { return m_val; }
    protected:
        virtual ~BooleanLiteralNode() {}

    AST_VISIT_THIS_NODE
};

class DeclarationNode: public ASTNode {
    private:
        // not using IdentifierNode because the name itself is not yet an expression
        std::string m_variableName;                     // variable name
        bool m_isConst;                                 // const qualified
        bool m_isReadOnly = false;                      // read-only qualified
        bool m_isWriteOnly = false;                     // write-only qualified
        int m_type;                                     // types defined in parser.tab.h
        ExpressionNode *m_initValExpr = nullptr;        // initial value expression (optional)
    public:
        DeclarationNode(const std::string &variableName, bool isConst, int type, ExpressionNode *initValExpr = nullptr):
            m_variableName(variableName), m_isConst(isConst), m_type(type), m_initValExpr(initValExpr) {}
    public:
        const std::string &getName() const { return m_variableName; }
        bool isConst() const { return m_isConst; }
        bool isReadOnly() const { return m_isReadOnly; }
        bool isWriteOnly() const { return m_isWriteOnly; }
        int getType() const { return m_type; }
        std::string getTypeString() const { return AST::getTypeString(m_type); }
        ExpressionNode *getExpression() const { return m_initValExpr; }
    public:
        /*
         * Attribute: Read-only, non-constant.
         * Uniform: Read-only, constant.
         * Result: Write-only, cannot be assigned anywhere in the scope of an if or else statement.
         */
        void setAttributeType() { m_isReadOnly = true; m_isConst = false; m_isWriteOnly = false; }
        void setUniformType() { m_isReadOnly = true; m_isConst = true; m_isWriteOnly = false; }
        void setResultType() { m_isReadOnly = false; m_isConst = false; m_isWriteOnly = true; }
        bool isAttributeType() const { return (m_isReadOnly == true && m_isConst == false && m_isWriteOnly == false); }
        bool isUniformType() const { return (m_isReadOnly == true && m_isConst == true && m_isWriteOnly == false); }
        bool isResultType() const { return (m_isReadOnly == false && m_isConst == false && m_isWriteOnly == true); }
        bool isOrdinaryType() const { return (m_isReadOnly == false && m_isWriteOnly == false); }
        std::string getQualifierString() const;
    protected:
        virtual ~DeclarationNode() {
            if(m_initValExpr != nullptr) {
                ASTNode::destructNode(m_initValExpr);
            }
        }
    
    AST_VISIT_THIS_NODE
};

class VariableNode: public ExpressionNode {
    /* Pure Virtual Intermediate Layer */
    public:
        virtual std::string getName() const = 0;
        virtual const DeclarationNode *getDeclaration() const = 0;
    public:
        virtual bool isConst() const = 0;
        virtual bool isReadOnly() const = 0;
        virtual bool isWriteOnly() const = 0;
        virtual bool isAttributeType() const = 0;
        virtual bool isUniformType() const = 0;
        virtual bool isResultType() const = 0;
        virtual bool isOrdinaryType() const = 0;
};

class IdentifierNode: public VariableNode {
    private:
        int m_type = ANY_TYPE;                          // types defined in parser.tab.h
        std::string m_id;                               // name of this identifier
        const DeclarationNode *m_decl = nullptr;        // declaration of this IdentifierNode
    public:
        IdentifierNode(const std::string &id):
            m_id(id) {}
    public:
        virtual int getExpressionType() const { return m_type; }
        virtual void setExpressionType(int type) { m_type = type; }
    public:
        virtual bool isConst() const { return m_decl ? m_decl->isConst() : false; }
        virtual bool isReadOnly() const { return m_decl ? m_decl->isReadOnly() : false; }
        virtual bool isWriteOnly() const { return m_decl ? m_decl->isWriteOnly() : false; }
        virtual bool isAttributeType() const { return m_decl ? m_decl->isAttributeType() : false; }
        virtual bool isUniformType() const { return m_decl ? m_decl->isUniformType() : false; }
        virtual bool isResultType() const { return m_decl ? m_decl->isResultType() : false; }
        virtual bool isOrdinaryType() const { return m_decl ? m_decl->isOrdinaryType() : true; }
    public:
        virtual std::string getName() const { return m_id; }
        virtual const DeclarationNode *getDeclaration() const { return m_decl; }
    public:
        void setDeclaration(const DeclarationNode *decl) { m_decl = decl; }
    protected:
        virtual ~IdentifierNode() {}

    AST_VISIT_THIS_NODE
};

class IndexingNode: public VariableNode {
    private:
        int m_type = ANY_TYPE;                          // types defined in parser.tab.h
        IdentifierNode *m_identifier;                   // identifier node of the vector variable
        ExpressionNode *m_indexExpr;                    // index expression node
    public:
        IndexingNode(IdentifierNode *identifier, ExpressionNode *indexExpr):
            m_identifier(identifier), m_indexExpr(indexExpr) {}
    public:
        virtual int getExpressionType() const { return m_type; }
        virtual void setExpressionType(int type) { m_type = type; }
    public:
        virtual bool isConst() const { return m_identifier->isConst(); }
        virtual bool isReadOnly() const { return m_identifier->isReadOnly(); }
        virtual bool isWriteOnly() const { return m_identifier->isWriteOnly(); }
        virtual bool isAttributeType() const { return m_identifier->isAttributeType(); }
        virtual bool isUniformType() const { return m_identifier->isUniformType(); }
        virtual bool isResultType() const { return m_identifier->isResultType(); }
        virtual bool isOrdinaryType() const { return m_identifier->isOrdinaryType(); }
    public:
        IdentifierNode *getIdentifier() const { return m_identifier; }
        ExpressionNode *getIndexExpression() const { return m_indexExpr; }
    public:
        virtual std::string getName() const {
            return m_identifier->getName() + "[" + std::to_string(reinterpret_cast<IntLiteralNode *>(m_indexExpr)->getVal()) + "]";
        }
        virtual const DeclarationNode *getDeclaration() const { return m_identifier->getDeclaration(); }
    protected:
        virtual ~IndexingNode() {
            ASTNode::destructNode(m_identifier);
            ASTNode::destructNode(m_indexExpr);
        }

    AST_VISIT_THIS_NODE
};

class FunctionNode: public ExpressionNode {
    private:
        int m_type = ANY_TYPE;                          // types defined in parser.tab.h
        std::string m_functionName;                     // name of this function
        ExpressionsNode *m_argExprs;                    // argument expressions of this function
    public:
        FunctionNode(const std::string &functionName, ExpressionsNode *argExprs):
            m_functionName(functionName), m_argExprs(argExprs) {}
    public:
        virtual int getExpressionType() const { return m_type; }
        virtual void setExpressionType(int type) { m_type = type; }
        virtual bool isConst() const { return false; }
    public:
        const std::string &getName() const { return m_functionName; }
        ExpressionsNode *getArgumentExpressions() const { return m_argExprs; }
    protected:
        virtual ~FunctionNode() {
            ASTNode::destructNode(m_argExprs);
        }

    AST_VISIT_THIS_NODE
};

class ConstructorNode: public ExpressionNode {
    private:
        int m_type = ANY_TYPE;                          // inferred expression type, types defined in parser.tab.h
        int m_constructorType;                          // actual type of this constructor
        bool m_isConst = false;                         // whether expression is const
        ExpressionsNode *m_argExprs;                    // argument expressions of this constructor
    public:
        ConstructorNode(int constructorType, ExpressionsNode *argExprs):
            m_constructorType(constructorType), m_argExprs(argExprs) {}
    public:
        virtual int getExpressionType() const { return m_type; }
        virtual void setExpressionType(int type) { m_type = type; }
        virtual bool isConst() const { return m_isConst; }
        virtual void setConst(bool isConst) { m_isConst = isConst; }
    public:
        int getConstructorType() const { return m_constructorType; }
        ExpressionsNode *getArgumentExpressions() const { return m_argExprs; }
    protected:
        virtual ~ConstructorNode() {
            ASTNode::destructNode(m_argExprs);
        }

    AST_VISIT_THIS_NODE
};

class StatementNode: public ASTNode {
    /* Pure Virtual Intermediate Layer */
    protected:
        virtual ~StatementNode() {}
};

class StatementsNode: public ASTNode {
    private:
        std::vector<StatementNode *> m_statements;      // a list of StatementNodes
    public:
        void pushBackStatement(StatementNode *stmt) { m_statements.push_back(stmt); }
        const std::vector<StatementNode *> &getStatementList() const { return m_statements; }
    protected:
        virtual ~StatementsNode() {
            for(StatementNode *stmt: m_statements) {
                ASTNode::destructNode(stmt);
            }
        }

    AST_VISIT_THIS_NODE
};

class DeclarationsNode: public ASTNode {
    private:
        std::list<DeclarationNode *> m_declarations;    // a list of DeclarationNodes
    public:
        void pushBackDeclaration(DeclarationNode *decl) { m_declarations.push_back(decl); }
        void pushFrontDeclaration(DeclarationNode *decl) { m_declarations.push_front(decl); }
        const std::list<DeclarationNode *> &getDeclarationList() const { return m_declarations; }
    protected:
        virtual ~DeclarationsNode() {
            for(DeclarationNode *decl: m_declarations) {
                ASTNode::destructNode(decl);
            }
        }

    AST_VISIT_THIS_NODE
};

class IfStatementNode: public StatementNode {
    private:
        ExpressionNode *m_condExpr;                     // condition expression
        StatementNode *m_thenStmt;                      // then statement
        StatementNode *m_elseStmt = nullptr;            // else statement (optional)
    public:
        IfStatementNode(ExpressionNode *condExpr, StatementNode *thenStmt, StatementNode *elseStmt = nullptr):
            m_condExpr(condExpr), m_thenStmt(thenStmt), m_elseStmt(elseStmt) {}
    public:
        ExpressionNode *getConditionExpression() const { return m_condExpr; }
        StatementNode *getThenStatement() const { return m_thenStmt; }
        StatementNode *getElseStatement() const { return m_elseStmt; }
    protected:
        virtual ~IfStatementNode() {
            ASTNode::destructNode(m_condExpr);
            ASTNode::destructNode(m_thenStmt);
            if(m_elseStmt != nullptr) {
                ASTNode::destructNode(m_elseStmt);
            }
        }
    
    AST_VISIT_THIS_NODE
};

class WhileStatementNode: public StatementNode {
    /* No Support */
    private:
        ExpressionNode *m_condExpr;                     // condition expression
        StatementNode *m_bodyStmt;                      // loop body statement
    public:
        WhileStatementNode(ExpressionNode *condExpr, StatementNode *bodyStmt):
            m_condExpr(condExpr), m_bodyStmt(bodyStmt) {}
    protected:
        virtual ~WhileStatementNode() {
            ASTNode::destructNode(m_condExpr);
            ASTNode::destructNode(m_bodyStmt);
        }
    
    AST_VISIT_THIS_NODE
};

class AssignmentNode: public StatementNode {
    private:
        int m_type = ANY_TYPE;                          // types defined in parser.tab.h
        VariableNode *m_var;                            // variable node
        ExpressionNode *m_newValExpr;                   // new value expression
    public:
        AssignmentNode(VariableNode *var, ExpressionNode *newValExpr):
            m_var(var), m_newValExpr(newValExpr) {}
    public:
        int getExpressionType() const { return m_type; }
        void setExpressionType(int type) { m_type = type; }
        std::string getExpressionTypeString() const { return getTypeString(m_type); }
        VariableNode *getVariable() const { return m_var; }
        ExpressionNode *getExpression() const { return m_newValExpr; }
    protected:
        virtual ~AssignmentNode() {
            ASTNode::destructNode(m_var);
            ASTNode::destructNode(m_newValExpr);
        }
    
    AST_VISIT_THIS_NODE
};

/* In case of a single semicolon statement */
class StallStatementNode: public StatementNode {
    public:
        StallStatementNode() {}
    protected:
        virtual ~StallStatementNode() {}

    AST_VISIT_THIS_NODE
};

/* Inner ScopeNode */
class NestedScopeNode: public StatementNode {
    private:
        DeclarationsNode *m_decls;
        StatementsNode *m_stmts;
    public:
        NestedScopeNode(DeclarationsNode *decls, StatementsNode *stmts):
            m_decls(decls), m_stmts(stmts) {}
    public:
        DeclarationsNode *getDeclarations() const { return m_decls; }
        StatementsNode *getStatements() const { return m_stmts; }
    protected:
        virtual ~NestedScopeNode() {
            ASTNode::destructNode(m_decls);
            ASTNode::destructNode(m_stmts);
        }

    AST_VISIT_THIS_NODE
};

/* Global ScopeNode */
class ScopeNode: public ASTNode {
    private:
        DeclarationsNode *m_decls;
        StatementsNode *m_stmts;
    public:
        ScopeNode(DeclarationsNode *decls, StatementsNode *stmts):
            m_decls(decls), m_stmts(stmts) {}
    public:
        DeclarationsNode *getDeclarations() const { return m_decls; }
        StatementsNode *getStatements() const { return m_stmts; }
    protected:
        virtual ~ScopeNode() {
            if(m_decls != nullptr) {
                ASTNode::destructNode(m_decls);
            }
            if(m_stmts != nullptr) {
                ASTNode::destructNode(m_stmts);
            }
        }
    
    public:
        static NestedScopeNode *convertToNestedScopeNode(ScopeNode *scopeNode) {
            NestedScopeNode *nestedScopeNode = new NestedScopeNode(scopeNode->m_decls, scopeNode->m_stmts);
            
            // Delete scopeNode
            scopeNode->m_decls = nullptr;
            scopeNode->m_stmts = nullptr;
            delete scopeNode;

            return nestedScopeNode;
        }

    AST_VISIT_THIS_NODE
};

}


typedef AST::ASTNode node;
extern node *ast;


typedef enum {
    UNKNOWN               = 0,

    SCOPE_NODE               ,

    EXPRESSION_NODE          ,
    EXPRESSIONS_NODE         ,
    UNARY_EXPRESION_NODE     ,
    BINARY_EXPRESSION_NODE   ,
    INT_C_NODE               ,
    FLOAT_C_NODE             ,
    BOOL_C_NODE              ,
    VAR_NODE                 ,
    ID_NODE                  ,
    INDEXING_NODE            ,
    FUNCTION_NODE            ,
    CONSTRUCTOR_NODE         ,

    STATEMENT_NODE           ,
    IF_STATEMENT_NODE        ,
    WHILE_STATEMENT_NODE     ,
    ASSIGNMENT_NODE          ,
    NESTED_SCOPE_NODE        ,
    STALL_STATEMENT_NODE     ,
    STATEMENTS_NODE          ,

    DECLARATION_NODE         ,
    DECLARATIONS_NODE
} node_kind;

node *ast_allocate(node_kind type, ...);
void ast_free(node *ast);
void ast_print(node * ast);

#endif /* AST_H_ */
