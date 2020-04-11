// ctc/export.h

#ifndef _CTC_EXPORT_H
#define _CTC_EXPORT_H 1

#include <stdint.h>
#include <limits.h>

//TODO: support cross compiling!

// Ruthlessly stolen from the Linux kernel source code:

#if UINTPTR_MAX > 0xFFFFFFFFFFFFFFFF
# warning "Cannot export symbols larger than 64 bits."
# define __CTC_XPTR_EXOTIC 1
#endif

#if defined(__GNUC__) && !defined(CTC_DISABLE_GNUC) && !defined(__CTC_XPTR_EXOTIC)
# define __CTC_SYMBOL_ATTRS __attribute__((packed))

# define __CTC_SYMTAB_SECTION_PREFIX "__ctc_symtab_"
# define __CTC_SYMTAB_STRINGS_SECTION __CTC_SYMTAB_SECTION_PREFIX "strings"
# define __CTC_SYMTAB_CLASSES_SECTION __CTC_SYMTAB_SECTION_PREFIX "class"
# define __CTC_SYMTAB_IFACES_SECTION __CTC_SYMTAB_SECTION_PREFIX "iface"

# define __CTC_SYMTAB_ENTRY(sym, sec) \
	static const ctc_symbol __ctc_symtab_##sym \
	__attribute__((section(sec), used, aligned(1))) \
	= { (uint64_t)&sym, (uint64_t)__ctc_strsymtab_##sym };

# define __CTC_SYMTAB_STRING(sym) \
	static const char __ctc_strsymtab_##sym[] \
	__attribute__((section(__CTC_SYMTAB_STRINGS_SECTION), used, aligned(1))) \
	= #sym;

# define __CTC_EXPORT(sym, sec) \
	extern typeof(sym) sym; \
	__CTC_SYMTAB_STRING(sym) \
	__CTC_SYMTAB_ENTRY(sym, sec);
#else
# define __CTC_EXPORT(sym, sec)
# define __CTC_SYMBOL_ATTRS
#endif

#ifndef CTC_EXPORT
# define CTC_EXPORT(sym, sec) __CTC_EXPORT(sym, sec)
#endif

#ifndef CTC_EXPORT_CLASS
# define CTC_EXPORT_CLASS(sym) CTC_EXPORT(sym, __CTC_SYMTAB_CLASSES_SECTION)
#endif

#ifndef CTC_EXPORT_IFACE
# define CTC_EXPORT_IFACE(sym) CTC_EXPORT(sym, __CTC_SYMTAB_IFACES_SECTION)
#endif

typedef uint64_t ctc_symbol[2];

#endif
