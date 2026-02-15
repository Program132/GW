#include "Statement.h"

Statement::Statement() : type(EXPRESSION_STATEMENT), token(Token()) {}

Statement::Statement(StatementType type) : type(type), token(Token()) {}

Statement::Statement(StatementType type, Token token)
    : type(type), token(token) {}

Statement::Statement(StatementType type, Expression expression)
    : type(type), token(Token()), expression(expression) {}

Statement::Statement(StatementType type, Token token, Expression expression)
    : type(type), token(token), expression(expression) {}

StatementType Statement::getType() const { return type; }
Token Statement::getToken() const { return token; }
Expression Statement::getExpression() const { return expression; }

std::ostream &operator<<(std::ostream &os, const Statement &statement) {
  switch (statement.getType()) {
  case EXPRESSION_STATEMENT:
    os << "EXPRESSION_STATEMENT";
    break;
  case PRINT_STATEMENT:
    os << "PRINT_STATEMENT";
    break;
  case PRINTLN_STATEMENT:
    os << "PRINTLN_STATEMENT";
    break;
  case VAR_DECLARATION:
    os << "VAR_DECLARATION";
    break;
  case BLOCK:
    os << "BLOCK";
    break;
  case IF:
    os << "IF";
    break;
  case WHILE:
    os << "WHILE";
    break;
  case FOR:
    os << "FOR";
    break;
  case FUNCTION_DECLARATION:
    os << "FUNCTION_DECLARATION";
    break;
  case RETURN:
    os << "RETURN";
    break;
  case STRUCT_DECLARATION:
    os << "STRUCT_DECLARATION";
    break;
  case BREAK:
    os << "BREAK";
    break;
  case CONTINUE:
    os << "CONTINUE";
    break;
  case CONSTRUCTOR_DECLARATION:
    os << "CONSTRUCTOR_DECLARATION";
    break;
  case CLASS_DECLARATION:
    os << "CLASS_DECLARATION";
    break;
  case OPERATOR_DECLARATION:
    os << "OPERATOR_DECLARATION";
    break;
  }
  return os;
}

ExpressionStatement::ExpressionStatement(Expression expression)
    : Statement(EXPRESSION_STATEMENT, expression) {}

PrintStatement::PrintStatement(Expression expression)
    : Statement(PRINT_STATEMENT, expression) {}

std::ostream &operator<<(std::ostream &os,
                         const PrintStatement &printStatement) {
  os << "PRINT_STATEMENT{" << printStatement.getExpression() << "}";
  return os;
}

PrintlnStatement::PrintlnStatement(Expression expression)
    : Statement(PRINTLN_STATEMENT, expression) {}

std::ostream &operator<<(std::ostream &os,
                         const PrintlnStatement &printlnStatement) {
  os << "PRINTLN_STATEMENT{" << printlnStatement.getExpression() << "}";
  return os;
}

VarDeclarationStatement::VarDeclarationStatement(Token name)
    : Statement(VAR_DECLARATION, name), name(name) {}

VarDeclarationStatement::VarDeclarationStatement(Token name,
                                                 Expression initializer)
    : Statement(VAR_DECLARATION, name, initializer), name(name),
      initializer(initializer) {}

Token VarDeclarationStatement::getName() const { return name; }

Expression VarDeclarationStatement::getInitializer() const {
  return initializer;
}

BlockStatement::BlockStatement(List<Statement> statements)
    : Statement(BLOCK), statements(statements) {}

List<Statement> BlockStatement::getStatements() const { return statements; }

IfStatement::IfStatement(Expression condition, Statement thenBranch)
    : Statement(IF), condition(condition), thenBranch(thenBranch) {}

IfStatement::IfStatement(Expression condition, Statement thenBranch,
                         Statement elseBranch)
    : Statement(IF), condition(condition), thenBranch(thenBranch),
      elseBranch(elseBranch) {}

Expression IfStatement::getCondition() const { return condition; }

Statement IfStatement::getThenBranch() const { return thenBranch; }

Statement IfStatement::getElseBranch() const { return elseBranch; }

WhileStatement::WhileStatement(Expression condition, Statement body)
    : Statement(WHILE), condition(condition), body(body) {}

Expression WhileStatement::getCondition() const { return condition; }

Statement WhileStatement::getBody() const { return body; }

ForStatement::ForStatement(Expression initializer, Expression condition,
                           Expression increment, Statement body)
    : Statement(FOR), initializer(initializer), condition(condition),
      increment(increment), body(body) {}

Expression ForStatement::getInitializer() const { return initializer; }

Expression ForStatement::getCondition() const { return condition; }

Expression ForStatement::getIncrement() const { return increment; }

Statement ForStatement::getBody() const { return body; }

FunctionDeclarationStatement::FunctionDeclarationStatement(
    Token name, List<List<Token>> params, BlockStatement body,
    DataTypes returnType)
    : Statement(FUNCTION_DECLARATION, name), name(name), params(params),
      body(body), returnType(returnType) {}

FunctionDeclarationStatement::FunctionDeclarationStatement(
    Token name, List<List<Token>> params, BlockStatement body)
    : Statement(FUNCTION_DECLARATION, name), name(name), params(params),
      body(body) {}

Token FunctionDeclarationStatement::getName() const { return name; }

List<List<Token>> FunctionDeclarationStatement::getParams() const {
  return params;
}

BlockStatement FunctionDeclarationStatement::getBody() const { return body; }

DataTypes FunctionDeclarationStatement::getReturnType() const {
  return returnType;
}

ReturnStatement::ReturnStatement() : Statement(RETURN), value() {}

ReturnStatement::ReturnStatement(Expression value)
    : Statement(RETURN), value(value) {}

Expression ReturnStatement::getValue() const { return value; }

StructDeclarationStatement::StructDeclarationStatement(Token name,
                                                       List<List<Token>> fields)
    : Statement(STRUCT_DECLARATION, name), name(name), fields(fields) {}

Token StructDeclarationStatement::getName() const { return name; }

List<List<Token>> StructDeclarationStatement::getFields() const {
  return fields;
}

BreakStatement::BreakStatement() : Statement(BREAK) {}

ContinueStatement::ContinueStatement() : Statement(CONTINUE) {}

ConstructorDeclarationStatement::ConstructorDeclarationStatement(
    List<Token> params, BlockStatement body)
    : Statement(CONSTRUCTOR_DECLARATION), params(params), body(body) {}

List<Token> ConstructorDeclarationStatement::getParams() const {
  return params;
}

BlockStatement ConstructorDeclarationStatement::getBody() const { return body; }

ClassDeclarationStatement::ClassDeclarationStatement(Token name,
                                                     List<List<Token>> fields)
    : Statement(CLASS_DECLARATION, name), name(name), fields(fields) {}

Token ClassDeclarationStatement::getName() const { return name; }

List<List<Token>> ClassDeclarationStatement::getFields() const {
  return fields;
}

OperatorDeclarationStatement::OperatorDeclarationStatement(
    Token op, List<List<Token>> params, BlockStatement body,
    DataTypes returnType)
    : Statement(OPERATOR_DECLARATION, op), op(op), params(params), body(body),
      returnType(returnType) {}

Token OperatorDeclarationStatement::getOperator() const { return op; }

List<List<Token>> OperatorDeclarationStatement::getParams() const {
  return params;
}

BlockStatement OperatorDeclarationStatement::getBody() const { return body; }

DataTypes OperatorDeclarationStatement::getReturnType() const {
  return returnType;
}