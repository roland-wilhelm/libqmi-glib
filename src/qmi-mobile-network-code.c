/*
 * qmi-mobile-network-code.c
 *
 *  Created on: 06.06.2012
 *      Author: roland
 */


#include "qmi-mobile-network-code.h"
#include <glib-object.h>


/*
 *
 * Country or Geographical Area Networks and enum values.
 */
static const GEnumValue qmi_mcc_types_values[] = {
    { GERMANY, "GERMANY", "Germany"},
    { 0, NULL, NULL }
};

static const GEnumValue qmi_mnc_types_values[] = {
    { T_MOBILE_DEUTSCHLAND_1, "T_MOBILE_DEUTSCHLAND", "T-Mobile"},
    { VODAFONE_D2_1, "VODAFONE_D2", "Vodafone D2"},
    { E_PLUS_MOBILFUNK_1, "E_PLUS_MOBILFUNK", "E-Plus"},
    { VODAFONE_D2_2, "VODAFONE_D2", "Vodafone D2"},
    { E_PLUS_MOBILFUNK_2, "E_PLUS_MOBILFUNK", "E-Plus"},
    { T_MOBILE_DEUTSCHLAND_2, "T_MOBILE_DEUTSCHLAND", "T-Mobile"},
    { O2_GERMANY_1, "O2_GERMANY", "O2"},
    { O2_GERMANY_2, "O2_GERMANY", "O2"},
    { VODAFONE_D2_3, "VODAFONE_D2", "Vodafone D2"},
    { ARCOR_1, "ARCOR", "Arcor"},
    { O2_GERMANY_3, "O2_GERMANY", "O2"},
    { 0, NULL, NULL }
};

const gchar* qmi_mcc_get_string(QmiMccEnum val) {

    guint i;

    for(i = 0; qmi_mcc_types_values[i].value_nick; i++) {
        if(val == qmi_mcc_types_values[i].value)
            return qmi_mcc_types_values[i].value_nick;
    }

    return NULL;
}

const gchar* qmi_mnc_get_string(QmiMncEnum val) {

    guint i;

    for(i = 0; qmi_mnc_types_values[i].value_nick; i++) {
        if(val == qmi_mnc_types_values[i].value)
            return qmi_mnc_types_values[i].value_nick;
    }

    return NULL;
}
