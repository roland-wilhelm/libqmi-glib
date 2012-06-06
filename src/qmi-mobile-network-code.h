/*
 * qmi-mobile-network-code.h
 *
 *  Created on: 06.06.2012
 *      Author: roland
 */

#ifndef _LIBQMI_GLIB_QMI_MOBILE_NETWORK_CODE_H_
#define _LIBQMI_GLIB_QMI_MOBILE_NETWORK_CODE_H_


#include <glib.h>

typedef enum {

	GERMANY		=	262,


}QmiMccEnum;

typedef enum {

	T_MOBILE_DEUTSCHLAND_1	=	1,
	VODAFONE_D2_1			=	2,
	E_PLUS_MOBILFUNK_1		=	3,
	VODAFONE_D2_2			=	4,
	E_PLUS_MOBILFUNK_2		=	5,
	T_MOBILE_DEUTSCHLAND_2	=	6,
	O2_GERMANY_1			=	7,
	O2_GERMANY_2			=	8,
	VODAFONE_D2_3			=	9,
	ARCOR_1					=	10,
	O2_GERMANY_3			=	11,


}QmiMncEnum;

const gchar* qmi_mcc_get_string(QmiMccEnum val);
const gchar* qmi_mnc_get_string(QmiMncEnum val);


#endif /* _LIBQMI_GLIB_QMI_MOBILE_NETWORK_CODE_H_ */
