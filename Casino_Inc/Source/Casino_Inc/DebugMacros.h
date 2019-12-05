#pragma once

//TODO:DOCUMENT comment/document this file

#if !UE_BUILD_SHIPPING

#include "Engine/Engine.h"

#define PRINT_EDITOR(text) if (GetWorld()->WorldType == EWorldType::Editor) if (GEngine)	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, text);
#define PRINT_EDITOR_KEY(key, text) if (GetWorld()->WorldType == EWorldType::Editor) if (GEngine)	GEngine->AddOnScreenDebugMessage(key, 5.0f, FColor::Red, text);
#define PRINT(text) if (GEngine)	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, text);
#define PRINT_KEY(key, text) if (GEngine)	GEngine->AddOnScreenDebugMessage(key, 5.0f, FColor::Red, text);
#define PRINT_COLOR(color, text) if (GEngine)	GEngine->AddOnScreenDebugMessage(-1, 5.0f, color, text);


/*
Joy String
Current Class, File, and Line Number!
by Rama

PreProcessor commands to get
a. Class name
b. Function Name
c. Line number
d. Function Signature (including parameters)

Gives you a UE4 FString anywhere in your code that these macros are used!

Ex:
You can use JOYSTR_CUR_CLASS anywhere to get a UE4 FString back telling you
what the current class is where you called this macro!

Ex:
This macro prints the class and line along with the message of your choosing!
PRINT_FLOW("Have fun today!");
<3  Rama
*/

//Current Class Name + Function Name where this is called!
#define JOYSTR_CUR_CLASS_FUNC (FString(__FUNCTION__))

//Current Class where this is called!
#define JOYSTR_CUR_CLASS (FString(__FUNCTION__).Left(FString(__FUNCTION__).Find(TEXT(":"))) )

//Current Function Name where this is called!
#define JOYSTR_CUR_FUNC (FString(__FUNCTION__).Right(FString(__FUNCTION__).Len() - FString(__FUNCTION__).Find(TEXT("::")) - 2 ))

//Current Line Number in the code where this is called!
#define JOYSTR_CUR_LINE  (FString::FromInt(__LINE__))

//Current Class and Line Number where this is called!
#define JOYSTR_CUR_CLASS_LINE (JOYSTR_CUR_CLASS + "(" + JOYSTR_CUR_LINE + ")")

//Current Function Signature where this is called!
#define JOYSTR_CUR_FUNCSIG (FString(__FUNCSIG__))

/*
 * This macro prints the class and line along with the message of your choosing!
 * PRINT_FLOW("Have fun today!");
 */
#define PRINT_FLOW(Param1) if (GEngine) (GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *(JOYSTR_CUR_CLASS_LINE + ": " + Param1)) );

#else

#define PRINT_EDITOR(text) ;
#define PRINT_EDITOR_KEY(key, text) ;
#define PRINT(text) ;
#define PRINT_KEY(key, text) ;
#define PRINT_COLOR(color, text) ;
#define PRINT_FLOW(Param1) ;

#endif
