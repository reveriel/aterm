/**
 * aterm1.h: Definition of the level 1 interface
 * of the ATerm library.
 */

#ifndef ATERM1_H
#define ATERM1_H

#include <stdio.h>
#include <stdarg.h>
#include "encoding.h"
#include "abool.h"
#include "atypes.h"

#ifdef __cplusplus
extern "C"
{
#endif/* __cplusplus */

#define	AT_FREE         0L
#define AT_APPL         1L
#define AT_INT          2L 
#define AT_REAL         3L
#define AT_LIST         4L
#define AT_PLACEHOLDER  5L
#define AT_BLOB         6L

#define AT_SYMBOL       7L

#define MAX_ARITY            256

struct __ATerm
{
  header_type   header;
  union _ATerm *next;
};

typedef union _ATerm
{
  header_type     header;
  struct __ATerm  aterm;
  union _ATerm*   subaterm[MAX_ARITY+3];
  MachineWord     word[MAX_ARITY+3];
} *ATerm;

/** The following functions implement the operations of
  * the 'standard' ATerm interface, and should appear
  * in some form in every implementation of the ATerm
  * datatype.
  * The prefix AT is used to make these functions
  * relatively unique.
  */

ATerm ATmake(const char *pattern, ...);
ATbool ATmatch(ATerm t, const char *pattern, ...);

ATerm ATmakeTerm(ATerm pat, ...);
ATbool ATmatchTerm(ATerm t, ATerm pat, ...);
ATerm ATvmake(const char *pat);
ATerm ATvmakeTerm(ATerm pat);
void  AT_vmakeSetArgs(va_list *args);
ATbool ATvmatch(ATerm t, const char *pat);
ATbool ATvmatchTerm(ATerm t, ATerm pat);

ATerm ATreadFromTextFile(FILE *file);
ATerm ATreadFromSharedTextFile(FILE *f);
ATerm ATreadFromBinaryFile(FILE *file);
ATerm ATreadFromFile(FILE *file);
ATerm ATreadFromNamedFile(const char *name);
ATerm ATreadFromString(const char *string);
ATerm ATreadFromSharedString(char *s, int size);
ATerm ATreadFromBinaryString(char *s, int size);

/* Abbreviation for ATreadFromString */
#define ATparse(s)	ATreadFromString((s))

/* int    ATgetType(ATerm t); */
#define ATgetType(t) GET_TYPE((t)->header)

/* ATbool ATisEqual(ATerm t1, ATerm t2); */
extern ATbool AT_isEqual(ATerm t1, ATerm t2);
extern ATbool AT_isDeepEqual(ATerm t1, ATerm t2);
#if defined(SEMI_DEEP_EQUALITY)
#define ATisEqual(t1,t2) (AT_isEqual((ATerm)(t1), (ATerm)(t2)))
/* The casts are needed because we want to allow the user
   to easily check the equality of other ATerm types like 
	 ATermList, ATermAppl, etc. */
#elif defined(DEEP_EQUALITY)
#define ATisEqual(t1,t2) (AT_isDeepEqual((ATerm)(t1), (ATerm)(t2)))
#else
#define ATisEqual(t1,t2) ((ATbool)((ATerm)(t1) == (ATerm)(t2)))
#endif
#define ATisEqualAFun(f1, f2) ((f1) == (f2))

ATbool ATisEqualModuloAnnotations(ATerm t1, ATerm t2);

ATbool ATwriteToTextFile(ATerm t, FILE *file);
long   ATwriteToSharedTextFile(ATerm t, FILE *f);
ATbool ATwriteToBinaryFile(ATerm t, FILE *file);
ATbool ATwriteToNamedTextFile(ATerm t, const char *name);
ATbool ATwriteToNamedSharedTextFile(ATerm t, const char *name);
ATbool ATwriteToNamedBinaryFile(ATerm t, const char *name);
char  *ATwriteToString(ATerm t);
char  *ATwriteToSharedString(ATerm t, int *len);
char  *ATwriteToBinaryString(ATerm t, int *len);
ATerm  ATsetAnnotation(ATerm t, ATerm label, ATerm anno);
ATerm  ATgetAnnotation(ATerm t, ATerm label);
ATerm  ATremoveAnnotation(ATerm t, ATerm label);


void ATprotect(ATerm *atp);
void ATunprotect(ATerm *atp);
void ATprotectArray(ATerm *start, unsigned int size);
void ATunprotectArray(ATerm *start);

/* Convenience macro's to circumvent gcc's (correct) warning:
 *   "dereferencing type-punned pointer will break strict-aliasing rules"
 * example usage: ATprotectList(&ATempty);
 */
#define ATprotectTerm(p) ATprotect((ATerm *)(void *)(p))
#define ATprotectList(p) ATprotect((ATerm *)(void *)(p))
#define ATprotectAppl(p) ATprotect((ATerm *)(void *)(p))
#define ATprotectPlaceholder(p) ATprotect((ATerm *)(void *)(p))
#define ATprotectInt(p) ATprotect((ATerm *)(void *)(p))
#define ATprotectReal(p) ATprotect((ATerm *)(void *)(p))

#define ATunprotectTerm(p) ATprotect((ATerm *)(void *)(p))
#define ATunprotectList(p) ATprotect((ATerm *)(void *)(p))
#define ATunprotectAppl(p) ATprotect((ATerm *)(void *)(p))
#define ATunprotectPlaceholder(p) ATprotect((ATerm *)(void *)(p))
#define ATunprotectInt(p) ATprotect((ATerm *)(void *)(p))
#define ATunprotectReal(p) ATprotect((ATerm *)(void *)(p))

/**
  * We also define some functions that are specific
  * for the C implementation of ATerms, but are part
  * of the level 1 interface nevertheless.
  */

void ATinit(int argc, char *argv[], ATerm *bottomOfStack);
void ATinitialize(int argc, char *argv[]);
ATbool ATisInitialized();
void ATsetWarningHandler(void (*handler)(const char *format, va_list args));
void ATsetErrorHandler(void (*handler)(const char *format, va_list args));
void ATsetAbortHandler(void (*handler)(const char *format, va_list args));
void ATwarning(const char *format, ...);
void ATerror(const char *format, ...);
void ATabort(const char *format, ...);
int  ATprintf(const char *format, ...);
int  ATfprintf(FILE *stream, const char *format, ...);
int  ATvfprintf(FILE *stream, const char *format, va_list args);

#ifdef __cplusplus
}
#endif/* __cplusplus */ 

#endif
