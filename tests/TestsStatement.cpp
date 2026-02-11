#include "TestsStatement.h"
#include "../src/lexer/Token/Token.h"
#include "../src/parser/ASTNodes/Statement.h"
#include "../src/utils/Assert.h"

void testStatement() {
  Token nameVar(TokenType::IDENTIFIER, "v", 1);
  Token num100(TokenType::NUMBER, "100", 1);
  LiteralExpression lit100(num100);

  // VarDeclarationStatement: v = 100
  VarDeclarationStatement varDecl1(nameVar, lit100);
  GW_ASSERT(varDecl1.getName().getValue() == "v");
  GW_ASSERT(varDecl1.getInitializer().getType() == ExpressionType::LITERAL);

  // VarDeclarationStatement: v (no initializer)
  VarDeclarationStatement varDecl2(nameVar);
  GW_ASSERT(varDecl2.getName().getValue() == "v");
  GW_ASSERT(varDecl2.getInitializer().getType() == ExpressionType::EMPTY_EXPR);

  // ExpressionStatement
  ExpressionStatement exprStmt(lit100);
  GW_ASSERT(exprStmt.getExpression().getType() == ExpressionType::LITERAL);

  // PrintStatement
  PrintStatement printStmt(lit100);
  GW_ASSERT(printStmt.getExpression().getType() == ExpressionType::LITERAL);

  // BlockStatement
  List<Statement> stmts;
  stmts.append(varDecl1);
  stmts.append(printStmt);
  BlockStatement block(stmts);
  GW_ASSERT(block.getStatements().size() == 2);

  // IfStatement (If-Then-Else)
  Token trueTok(TokenType::BOOLEAN, "true", 1);
  LiteralExpression cond(trueTok);
  IfStatement ifElse(cond, printStmt, exprStmt);
  GW_ASSERT(ifElse.getCondition().getType() == ExpressionType::LITERAL);

  // WhileStatement
  WhileStatement whileStmt(cond, block);
  GW_ASSERT(whileStmt.getCondition().getType() == ExpressionType::LITERAL);

  // ForStatement: for(i; i < 10; i++)
  ForStatement forStmt(lit100, cond, lit100, printStmt);
  GW_ASSERT(forStmt.getInitializer().getType() == ExpressionType::LITERAL);

  // FunctionDeclarationStatement (Int result, no params)
  List<List<Token>> emptyParams;
  FunctionDeclarationStatement func1(nameVar, emptyParams, block,
                                     DataTypes::Int);
  GW_ASSERT(func1.getReturnType() == DataTypes::Int);
  GW_ASSERT(func1.getParams().size() == 0);

  // ReturnStatement
  ReturnStatement retVal(lit100);
  GW_ASSERT(retVal.getValue().getType() == ExpressionType::LITERAL);

  ReturnStatement retEmpty;
  GW_ASSERT(retEmpty.getValue().getType() == ExpressionType::EMPTY_EXPR);

  // StructDeclarationStatement
  List<List<Token>> fields;
  List<Token> field1;
  field1.append(Token(TokenType::INT, "int", 1));
  field1.append(Token(TokenType::IDENTIFIER, "age", 1));
  fields.append(field1);
  StructDeclarationStatement structDecl(
      Token(TokenType::IDENTIFIER, "Person", 1), fields);
  GW_ASSERT(structDecl.getFields().size() == 1);
  GW_ASSERT(structDecl.getName().getValue() == "Person");

  // ClassDeclarationStatement
  ClassDeclarationStatement classDecl(
      Token(TokenType::IDENTIFIER, "MyClass", 1), fields);
  GW_ASSERT(classDecl.getName().getValue() == "MyClass");

  // Operators, Break, Continue
  OperatorDeclarationStatement opDecl(Token(TokenType::MATH_OPERATOR, "+", 1),
                                      emptyParams, block, DataTypes::Int);
  GW_ASSERT(opDecl.getOperator().getValue() == "+");

  BreakStatement b;
  ContinueStatement c;
}
