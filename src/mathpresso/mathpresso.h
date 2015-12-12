// [MathPresso]
// Mathematical Expression Parser and JIT Compiler.
//
// [License]
// Zlib - See LICENSE.md file in the package.

#ifndef _MATHPRESSO_H
#define _MATHPRESSO_H

namespace mathpresso {

// ============================================================================
// [MathPresso - Configuration]
// ============================================================================

// EMBED implies STATIC.
#if defined(MATHPRESSO_EMBED) && !defined(MATHPRESSO_STATIC)
# define MATHPRESSO_STATIC
#endif

#if !defined(MATHPRESSO_API)
# if defined(MATHPRESSO_STATIC)
#  define MATHPRESSO_API
# elif defined(_WINDOWS)
#  if defined(__GNUC__) || defined(__clang__) && !defined(__MINGW32__)
#   if defined(MATHPRESSO_EXPORTS)
#    define MATHPRESSO_API __attribute__((__dllexport__))
#   else
#    define MATHPRESSO_API __attribute__((__dllimport__))
#   endif
#  else
#   if defined(MATHPRESSO_EXPORTS)
#    define MATHPRESSO_API __declspec(dllexport)
#   else
#    define MATHPRESSO_API __declspec(dllimport)
#   endif
#  endif
# else
#  if defined(__clang__) || defined(__GNUC__)
#   define MATHPRESSO_API __attribute__((__visibility__("default")))
#  endif
# endif
#endif

//! Mathpresso hidden API decorator.
//!
//! Purpose of this macro is to mark API that shouldn't be exported.
#define MATHPRESSO_NOAPI

//! Get an offset of `field` in a struct `type`.
#define MATHPRESSO_OFFSET(type, field) ((int)(size_t) ((const char*) &((const type*)0x10)->field) - 0x10)

// ============================================================================
// [MathPresso - Typedefs]
// ============================================================================

//! MathPresso result type (signed integer).
typedef int MPResult;

//! Prototype of function generated by MathPresso.
typedef void (*MPEvalFunc)(const void* priv, double* retval, void* data);

// ============================================================================
// [MathPresso - Result Codes]
// ============================================================================

//! MathPresso result codes.
enum MPResultCode {
  //! No error.
  kMPResultOk = 0,
  //! No memory.
  kMPResultNoMemory = 1,
  //! No expression.
  kMPResultNoExpression = 2,
  //! Unknown variable.
  kMPResultNoSymbol = 3,

  kMPResultInvalidArgument = 4,
  //! Invalid token.
  kMPResultInvalidToken = 5,
  //! Unexpected token.
  kMPResultUnexpectedToken = 6,
  //! Expected expression.
  kMPResultExpectedExpression = 7,
  //! Assignment to non-variable error.
  kMPResultAssignmentToNonVariable = 8,
  //! Assignment inside expression error.
  kMPResultAssignmentInsideExpression = 9,
  //! Function arguments mismatch.
  kMPResultArgumentsMismatch = 10,

  //! JIT compiler error.
  kMPResultJITError = 11
};

// ============================================================================
// [MathPresso - Environment]
// ============================================================================

//! MathPresso environment definitions.
enum MPEnvironment {
  kMPEnvironmentAll = 0,
  kMPEnvironmentMath = 1,
  kMPEnvironmentCount = 2
};

// ============================================================================
// [MathPresso - Options]
// ============================================================================

//! MathPresso options.
enum MPOptions {
  //! No options.
  kMPOptionNone = 0x00000000,
  //! Disable JIT compiler.
  kMPOptionDisableJIT = 0x00000001,
  //! Print JIT code generated.
  kMPOptionDumpJIT = 0x00000002
};

// ============================================================================
// [MathPresso - Variables]
// ============================================================================

//! Variable flags.
enum MPVarFlags {
  kMPVarRW = 0x00000000,
  kMPVarRO = 0x00000001
};

// ============================================================================
// [MathPresso - Functions]
// ============================================================================

enum MPFuncFlags {
  // --------------------------------------------------------------------------
  // [Flags - Bit combinations that can be used by function prototype]
  // --------------------------------------------------------------------------

  //! Function doesn't return any value (void).
  kMPFuncVoid = 0x00000100,

  //! Function is safe - can be evaluated by an expression simplifier.
  //!
  //! If this flag is used then function can be called during 
  //! an Expression::create() to evaluate constant expressions.
  kMPFuncSafe = 0x00000200,

  //! Function arguments and/or return values are of `double` type.
  kMPFuncDouble = 0x00000400,

  // --------------------------------------------------------------------------
  // [Prototype - Combination of bit flags and function arguments]
  // --------------------------------------------------------------------------

  kMPFuncProtoVoidDouble0 = (kMPFuncVoid | kMPFuncDouble) + 0,
  kMPFuncProtoVoidDouble1 = (kMPFuncVoid | kMPFuncDouble) + 1,
  kMPFuncProtoVoidDouble2 = (kMPFuncVoid | kMPFuncDouble) + 2,
  kMPFuncProtoVoidDouble3 = (kMPFuncVoid | kMPFuncDouble) + 3,
  kMPFuncProtoVoidDouble4 = (kMPFuncVoid | kMPFuncDouble) + 4,
  kMPFuncProtoVoidDouble5 = (kMPFuncVoid | kMPFuncDouble) + 5,
  kMPFuncProtoVoidDouble6 = (kMPFuncVoid | kMPFuncDouble) + 6,
  kMPFuncProtoVoidDouble7 = (kMPFuncVoid | kMPFuncDouble) + 7,
  kMPFuncProtoVoidDouble8 = (kMPFuncVoid | kMPFuncDouble) + 8,

  kMPFuncProtoDoubleDouble0 = (kMPFuncDouble) + 0,
  kMPFuncProtoDoubleDouble1 = (kMPFuncDouble) + 1,
  kMPFuncProtoDoubleDouble2 = (kMPFuncDouble) + 2,
  kMPFuncProtoDoubleDouble3 = (kMPFuncDouble) + 3,
  kMPFuncProtoDoubleDouble4 = (kMPFuncDouble) + 4,
  kMPFuncProtoDoubleDouble5 = (kMPFuncDouble) + 5,
  kMPFuncProtoDoubleDouble6 = (kMPFuncDouble) + 6,
  kMPFuncProtoDoubleDouble7 = (kMPFuncDouble) + 7,
  kMPFuncProtoDoubleDouble8 = (kMPFuncDouble) + 8
};

// ============================================================================
// [MathPresso - Context]
// ============================================================================

//! MathPresso context.
//!
//! Context is environment where you can add/remove constants, variables and
//! functions. Context is internally reference-counted class that is using
//! copy-on-write (cow). Working with context is reentrant and making weak
//! or deep copy is thread-safe (reference counting is atomic).
//!
//! It is possible to create one master context and use it from different
//! threads for many expressions.
struct Context {
  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  //! Create a new `Context` instance.
  MATHPRESSO_API Context();
  //! Create a new `Context` based on `other`.
  MATHPRESSO_API Context(const Context& other);
  //! Destroy the `Context` instance.
  MATHPRESSO_API ~Context();

  //! Add math constants and functions.
  MATHPRESSO_API MPResult addEnvironment(int environmentId);

  //! Add function to this context.
  MATHPRESSO_API MPResult addFunction(const char* name, void* ptr, int prototype);
  //! Add constant to this context.
  MATHPRESSO_API MPResult addConstant(const char* name, double value);
  //! Add variable to this context.
  MATHPRESSO_API MPResult addVariable(const char* name, int offset, int flags = kMPVarRW);

  //! Delete symbol from this context.
  MATHPRESSO_API MPResult delSymbol(const char* name);
  //! Delete all symbols.
  MATHPRESSO_API MPResult clear();

  //! Assignement operator.
  MATHPRESSO_API Context& operator=(const Context& other);

  // --------------------------------------------------------------------------
  // [Members]
  // --------------------------------------------------------------------------

  //! Private data not available to the MathPresso public API.
  void* _privateData;
};

// ============================================================================
// [MathPresso - Expression]
// ============================================================================

//! MathPresso expression.
struct Expression {
  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  //! Create a new `Expression` instance.
  MATHPRESSO_API Expression();
  //! Destroy the `Expression` instance.
  MATHPRESSO_API ~Expression();

  // --------------------------------------------------------------------------
  // [Methods]
  // --------------------------------------------------------------------------

  //! Compile a given `expression`.
  //!
  //! \param expression Expression to parse.
  //! \param variableName Name of variables inside expression.
  //! \param variableCount Count of variables in `variableName` array.
  //! \param options MathPresso options (flags), see \ref MPOptions.
  //!
  //! \return MathPresso result (see \ref MPResult).
  MATHPRESSO_API MPResult create(const Context& ectx, const char* expression, int options = kMPOptionNone);

  //! Free the expression.
  MATHPRESSO_API void free();

  //! Evaluate expression with variable substitutions.
  //!
  //! \return Result of evaluated expression, otherwise 0.0.
  inline double evaluate(void* data) const {
    double result;
    _evaluate(_privateData, &result, data);
    return result;
  }

  // --------------------------------------------------------------------------
  // [Members]
  // --------------------------------------------------------------------------

  //! Private data not available to the MathPresso public API.
  void* _privateData;

  //! Compiled expression.
  MPEvalFunc _evaluate;

private:
  // DISABLE COPY of Expression instance.
  inline Expression(const Expression& other);
  inline Expression& operator=(const Expression& other);
};

} // mathpresso namespace

#endif // _MATHPRESSO_H