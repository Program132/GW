#include "TestsExpression.h"
#include "../src/lexer/Token/Token.h"
#include "../src/parser/ASTNodes/Expression.h"
#include "../src/utils/Assert.h"

void testExpression() {
  Token number42(TokenType::NUMBER_TOKEN, "42", 1);
  Token number10(TokenType::NUMBER_TOKEN, "10", 1);
  Token plusToken(TokenType::MATH_OPERATOR, "+", 1);
  Token starToken(TokenType::MATH_OPERATOR, "*", 1);
  Token nameX(TokenType::IDENTIFIER, "x", 1);
  Token nameY(TokenType::IDENTIFIER, "y", 1);

  // Literal Expression
  LiteralExpression literal42(number42);
  GW_ASSERT(literal42.getType() == ExpressionType::LITERAL);
  GW_ASSERT(literal42.getValue().getValue() == "42");

  LiteralExpression literalTrue(Token(TokenType::BOOLEAN_TOKEN, "true", 1));
  GW_ASSERT(literalTrue.getValue().getValue() == "true");

  // Variable Expression
  VariableExpression varX(nameX);
  GW_ASSERT(varX.getType() == ExpressionType::VARIABLE);
  GW_ASSERT(varX.getName().getValue() == "x");

  // Binary Expression: x + 42
  BinaryExpression binaryAdd(new VariableExpression(nameX), plusToken,
                             new LiteralExpression(number42));
  GW_ASSERT(binaryAdd.getOp().getValue() == "+");
  GW_ASSERT(binaryAdd.getLeft()->getType() == ExpressionType::VARIABLE);

  // Complex Binary: (x + 42) * 10
  BinaryExpression complexBinary(
      new BinaryExpression(new VariableExpression(nameX), plusToken,
                           new LiteralExpression(number42)),
      starToken, new LiteralExpression(number10));
  GW_ASSERT(complexBinary.getOp().getValue() == "*");
  GW_ASSERT(complexBinary.getLeft()->getType() == ExpressionType::BINARY);

  // Unary Expression: -x
  UnaryExpression unary(Token(TokenType::MATH_OPERATOR, "-", 1),
                        new VariableExpression(nameX));
  GW_ASSERT(unary.getType() == ExpressionType::UNARY);
  GW_ASSERT(unary.getOp().getValue() == "-");

  // Grouping Expression: (x)
  GroupingExpression grouping(new VariableExpression(nameX));
  GW_ASSERT(grouping.getExpression()->getType() == ExpressionType::VARIABLE);

  // Assign Expression: x = 10
  AssignExpression assign(nameX, new LiteralExpression(number10));
  GW_ASSERT(assign.getName().getValue() == "x");
  GW_ASSERT(assign.getValue()->getType() == ExpressionType::LITERAL);

  // Call Expression: x(10, 42)
  List<Expression *> args;
  args.append(new LiteralExpression(number10));
  args.append(new LiteralExpression(number42));
  CallExpression call(new VariableExpression(nameX),
                      Token(TokenType::OPERATOR, "(", 1), args);
  GW_ASSERT(call.getArguments().size() == 2);
  GW_ASSERT(call.getArguments().get(1)->getType() == ExpressionType::LITERAL);

  // Get Expression: x.y
  GetExpression getExpr(new VariableExpression(nameX), nameY);
  GW_ASSERT(getExpr.getType() == ExpressionType::GET);
  GW_ASSERT(getExpr.getName().getValue() == "y");

  // Set Expression: x.y = 42
  SetExpression setExpr(new VariableExpression(nameX), nameY,
                        new LiteralExpression(number42));
  GW_ASSERT(setExpr.getType() == ExpressionType::SET);
  GW_ASSERT(setExpr.getName().getValue() == "y");
  GW_ASSERT(setExpr.getValue()->getType() == ExpressionType::LITERAL);

  // Nested Set: x.y = z.a
  VariableExpression *varZ =
      new VariableExpression(Token(TokenType::IDENTIFIER, "z", 1));
  GetExpression *getZA =
      new GetExpression(varZ, Token(TokenType::IDENTIFIER, "a", 1));
  SetExpression nestedSet(new VariableExpression(nameX), nameY, getZA);
  GW_ASSERT(nestedSet.getValue()->getType() == ExpressionType::GET);

  // Empty Expression (The one we added to fix the bug!)
  Expression empty;
  GW_ASSERT(empty.getType() == ExpressionType::EMPTY_EXPR);
}
