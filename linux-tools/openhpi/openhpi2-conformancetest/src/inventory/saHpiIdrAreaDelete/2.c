/*      -*- linux-c -*-
 *
 * (C) Copyright IBM Corp. 2004, 2005
 *
 *   This program is free software; you can redistribute it and/or modify 
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or 
 *   (at your option) any later version.
 *   This program is distributed in the hope that it will be useful, 
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *   GNU General Public License for more details. 
 *   You should have received a copy of the GNU General Public License 
 *   along with this program; if not, write to the Free Software 
 *   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 
 *   USA 
 *
 * Author(s):
 *      Carl McAdams <carlmc@us.ibm.com>
 *      Donald A. Barre <dbarre@unh.edu>
 *
 * Spec:        HPI-B.01.01
 * Function:    saHpiIdrAreaDelete
 * Description:
 *   Pass in an invalid SessionID.
 *   Expected return: SA_ERR_HPI_INVALID_SESSION.
 * Line:        P29-44:P29-46
 */

#include <stdio.h>
#include "../include/inventory_test.h"

/*************************************************************************
 *
 * Test an invalid session id.
 *
 *************************************************************************/

int processInventoryRdr(SaHpiSessionIdT sessionId,
			SaHpiResourceIdT resourceId,
			SaHpiRdrT * rdr, SaHpiInventoryRecT * inventoryRec)
{
	SaErrorT status;
	int retval;
	SaHpiEntryIdT NextAreaId;
	SaHpiIdrAreaHeaderT Header;

	// Since we need to use a valid AreaId in the call to
	// saHpiIdrAreaDelete(), let's use the AreaId of
	// the first area entry in the IDR if there is one.

	status =
	    saHpiIdrAreaHeaderGet(sessionId, resourceId, inventoryRec->IdrId,
				  SAHPI_IDR_AREATYPE_UNSPECIFIED,
				  SAHPI_FIRST_ENTRY, &NextAreaId, &Header);

	if (status == SA_ERR_HPI_NOT_PRESENT) {
		retval = SAF_TEST_NOTSUPPORT;
	} else if (status != SA_OK) {
		retval = SAF_TEST_UNRESOLVED;
		e_print(saHpiIdrAreaHeaderGet, SA_OK, status);
	} else {

		// Call saHpiIdrAreaDelete() passing in an invalid SessionId

		status = saHpiIdrAreaDelete(INVALID_SESSION_ID,
					    resourceId,
					    inventoryRec->IdrId, Header.AreaId);

		if (status == SA_ERR_HPI_INVALID_SESSION) {
			retval = SAF_TEST_PASS_AND_EXIT;
		} else {
			retval = SAF_TEST_FAIL;
			e_print(saHpiIdrAreaDelete, SA_ERR_HPI_INVALID_SESSION,
				status);
		}

	}

	return retval;
}

/*************************************************************************
 *
 *  Process all Inventory RDRs.  The below macro expands to
 *  generate all of the generic codes necessary to call the given
 *  function to process an RDR.
 *
 *************************************************************************/

processAllInventoryRdrs(processInventoryRdr)
