// *****************************************************************************
//
//     Copyright (c) 2015, Pleora Technologies Inc., All rights reserved.
//
// *****************************************************************************

#ifndef __PVTAPGEOMETRY_H__
#define __PVTAPGEOMETRY_H__

///
/// \file PvTapGeometry.h
/// \brief Tap geometry types definition.
///

///
/// \enum PvTapGeometryEnum
/// \brief Tap geoemtry enumeration type.
///
/// Refer to the GenICam Standard Feature Naming Convention for more information
/// on the tap configurations defined by this enum.
///

/// \var PvTapGeometryAS_2X_1Y
/// 2 zones in X, 1 zone in Y.

/// \var PvTapGeometryAS_2XE_1Y
/// 2 zones in X with end extraction, 1 zone in Y.

/// \var PvTapGeometryAS_2XM_1Y
/// 2 zones in X with middle extraction, 1 zone in Y.

/// \var PvTapGeometryAS_1X_1Y2
/// 1 zone in X, 1 zone in Y with 2 taps.

/// \var PvTapGeometryAS_1X_2YE
/// 1 zone in X, 2 zones in Y with end extraction.

/// \var PvTapGeometryAS_4X_1Y
/// 4 zones in X, 1 zone in Y.

/// \var PvTapGeometryAS_2X2_1Y
/// 2 zones in X with 2 taps, 1 zone in Y.

/// \var PvTapGeometryAS_2X2M_1Y
/// 2 zones in X with 2 taps and middle extraction, 1 zone in Y.

/// \var PvTapGeometryAS_1X2_2YE
/// 1 zone in X with 2 taps, 2 zones in Y with end extraction.

/// \var PvTapGeometryAS_1X2_1Y2
/// 1 zone in X with 2 taps, 1 zone in Y with 2 taps.

/// \var PvTapGeometryAS_2X_2YE
/// 2 zones in X, 2 zones in Y with end extraction.

/// \var PvTapGeometryAS_2X_1Y2
/// 2 zones in X, 1 zone in Y with 2 taps.

/// \var PvTapGeometryAS_2X2E_1Y
/// 2 zones in X with 2 taps, 1 zone in Y.

/// \var PvTapGeometryAS_2XE_2YE
/// 2 zones in X with end extraction, 2 zones in Y with end extraction.

/// \var PvTapGeometryAS_2XE_1Y2
/// 2 zones in X with end extraction, 1 zone in Y with 2 taps.

/// \var PvTapGeometryAS_2XM_2YE
/// 2 zones in X with middle extraction, 2 zones in Y with end extraction.

/// \var PvTapGeometryAS_2XM_1Y2
/// 2 zones in X with middle extraction,  1 zone in Y with 2 taps.

/// \var PvTapGeometryAS_8X_1Y
/// 8 zones in X, 1 zone in Y.

/// \var PvTapGeometryAS_4X2_1Y
/// 4 zones in X with 2 taps, 1 zone in Y.

/// \var PvTapGeometryAS_4X2E_1Y
/// 4 zones in X with 2 taps, 2 zones in Y with 2 taps.

/// \var PvTapGeometryAS_2X2E_2YE
/// 2 zones in X with 2 taps, 2 zones in Y with 2 taps.

/// \var PvTapGeometryLS_2X
/// Linescan, 2 zones in X. Same as #PvTapGeometryAS_2X_1Y.

/// \var PvTapGeometryLS_2XE
/// Linescan, 2 zones in X with end extraction. Same as #PvTapGeometryAS_2XE_1Y.

/// \var PvTapGeometryLS_2XM
/// Linescan, 2 zones in X with middle extraction. Same as #PvTapGeometryAS_2XM_1Y.

/// \var PvTapGeometryLS_4X
/// Linescan, 4 zones in X. Same as #PvTapGeometryAS_4X_1Y.

/// \var PvTapGeometryLS_2X2
/// Linescan, 2 zones in X with 2 taps. Same as #PvTapGeometryAS_2X2_1Y.

/// \var PvTapGeometryLS_2X2E
/// Linescan, 2 zones in X with 2 taps and end extraction. Same as #PvTapGeometryAS_2X2E_1Y.

/// \var PvTapGeometryLS_2X2M
/// Linescan, 2 zones in X with 2 taps and middle extraction. Same as #PvTapGeometryAS_2X2M_1Y.

/// \var PvTapGeometryLS_8X
/// Linescan, 8 zones in X. Same as PvTapGeometryAS_8X_1Y.

/// \var PvTapGeometryLS_4X2
/// Linescan, 4 zones in X with 2 taps. Same as #PvTapGeometryAS_4X2_1Y.

/// \var PvTapGeometryLS_4X2E
/// Linescan, 4 zones in X with 2 taps and end extraction. Same as #PvTapGeometryAS_4X2E_1Y.

typedef enum
{
    PvTapGeometryUnknown = -1,

    // Area scan
    PvTapGeometryAS_2X_1Y = 0,
    PvTapGeometryAS_2XE_1Y,
    PvTapGeometryAS_2XM_1Y,
    PvTapGeometryAS_1X_1Y2,
    PvTapGeometryAS_1X_2YE,
    PvTapGeometryAS_4X_1Y,
    PvTapGeometryAS_2X2_1Y,
    PvTapGeometryAS_2X2M_1Y,
    PvTapGeometryAS_1X2_2YE,
    PvTapGeometryAS_1X2_1Y2,
    PvTapGeometryAS_2X_2YE,
    PvTapGeometryAS_2X_1Y2,
    PvTapGeometryAS_2X2E_1Y,
    PvTapGeometryAS_2XE_2YE,
    PvTapGeometryAS_2XE_1Y2,
    PvTapGeometryAS_2XM_2YE,
    PvTapGeometryAS_2XM_1Y2,
    PvTapGeometryAS_8X_1Y,
    PvTapGeometryAS_4X2_1Y,
    PvTapGeometryAS_4X2E_1Y,
    PvTapGeometryAS_2X2E_2YE,

    // Linescan
    PvTapGeometryLS_2X,
    PvTapGeometryLS_2XE,
    PvTapGeometryLS_2XM,
    PvTapGeometryLS_4X,
    PvTapGeometryLS_2X2,
    PvTapGeometryLS_2X2E,
    PvTapGeometryLS_2X2M,
    PvTapGeometryLS_8X,
    PvTapGeometryLS_4X2,
    PvTapGeometryLS_4X2E,

    PvTapGeometryLast

} PvTapGeometryEnum;


#endif

