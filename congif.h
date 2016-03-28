#ifndef CONGIF_H
#define CONGIF_H

#include "math.h"

#define kBtnTextDrawLine "Нарисовать прямую"
#define kBtnTextDrawSpectr "Нарисовать спектр"
#define kBtnTextDrawClear "Отчистить"

#define kTextTitle "Лабараторная работа 3"

#define kLabelAlorithm "Выбор алгоритма"
#define kLabelSpectr "Спектр"
#define kLabelLine "Прямая"

#define kRadioTextDDA "ЦДА"
#define kRadioTextFloatBr "Действительный Брезенхем"
#define kRadioTextIntBr "Целочисленный Брезенхем"
#define kRadioTextSmtBr "Сглаживание"
#define kRadioTextQt "Qt"

#define kPlaceHolderSpectrX "Координата X"
#define kPlaceHolderSpectrY "Координата Y"
#define kPlaceHolderSpectrLineCount "Количество лучей"

#define kPlaceHolderLineX1 "Координата X1"
#define kPlaceHolderLineY1 "Координата Y1"
#define kPlaceHolderLineX2 "Координата X2"
#define kPlaceHolderLineY2 "Координата Y2"

#define kErrorMsgIncorrectData "Введите корректные данные!"

#define kDrawPointRadius 3
#define kDrawPenWidth 1

#define kmin(a, b) ( ( a < b) ? a : b )
#define kmax(a, b) ( ( a > b) ? a : b )
#define kRadianToDegree(radian) ( radian*180/M_PI )
#define kDegreeToRadian(degree) ( degree*M_PI/180 )

#endif // CONGIF_H
