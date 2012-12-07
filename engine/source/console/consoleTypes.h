//-----------------------------------------------------------------------------
// Torque
// Copyright GarageGames, LLC 2011
//-----------------------------------------------------------------------------

#ifndef _CONSOLETYPES_H_
#define _CONSOLETYPES_H_

#ifndef _CONSOLE_BASE_TYPE_H_
#include "console/consoleBaseType.h"
#endif

#ifndef Offset
#if defined(TORQUE_COMPILER_GCC) && (__GNUC__ > 3) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1))
#define Offset(m,T) ((int)(&((T *)1)->m) - 1)
#else
#define Offset(x, cls) ((dsize_t)((const char *)&(((cls *)0)->x)-(const char *)0))
#endif
#endif

// Define Core Console Types
DefineConsoleType( TypeF32 )
DefineConsoleType( TypeS8 )
DefineConsoleType( TypeS32 )
DefineConsoleType( TypeS32Vector )
DefineConsoleType( TypeBool )
DefineConsoleType( TypeBoolVector )
DefineConsoleType( TypeF32Vector )
DefineConsoleType( TypeString )
DefineConsoleType( TypeStringTableEntryVector )
DefineConsoleType( TypeCaseString )
DefineConsoleType( TypeFilename )
DefineConsoleType( TypeEnum )
DefineConsoleType( TypeFlag )
DefineConsoleType( TypeColorI )
DefineConsoleType( TypeColorF )
DefineConsoleType( TypeSimObjectPtr )
DefineConsoleType( TypeSimObjectName )
DefineConsoleType( TypeT2DVector )

#endif
