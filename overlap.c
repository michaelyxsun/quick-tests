#include <stdatomic.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef LOCAL
#include "/Users/msun/rehs2026/cuest/libcuest-linux-sbsa-0.1.1.1_cuda13-archive/include/cuest.h"
#else
#include <cuest.h>
#endif

#define CUESTAPI
#include "helper_status.h"
#include "helper_workspace.h"
#include "util.h"

#include "basis_H2O_STO-3G.h"

int
main ()
{
    // ============= //
    // set up handle //
    // ============= //

    cuestHandle_t           handle;
    cuestHandleParameters_t handle_params;
    checkCuestErrors (
        cuestParametersCreate (CUEST_HANDLE_PARAMETERS, &handle_params));
    checkCuestErrors (cuestCreate (handle_params, &handle));
    checkCuestErrors (
        cuestParametersDestroy (CUEST_HANDLE_PARAMETERS, handle_params));

    // ================ //
    // set up AO shells //
    // ================ //

    const uint64_t natom               = 3;           // used for basis
    const uint64_t nshells_per_atom[3] = { 3, 1, 1 }; // used for basis
    const uint64_t nshell              = 5;

    cuestAOShell_t *shells = malloc (nshell * sizeof (cuestAOShell_t));
    if (!shells) {
        fprintf (stderr, "Failed to allocate AO shell array\n");
        checkCuestErrors (cuestDestroy (handle));
        exit (EXIT_FAILURE);
    }

    cuestAOShellParameters_t aoshell_params;
    checkCuestErrors (
        cuestParametersCreate (CUEST_AOSHELL_PARAMETERS, &aoshell_params));

    for (size_t i = 0; i < nshell; ++i) {
        uint64_t ifirst
            = quick_basis.first_basis_function[quick_basis.katom[i] - 1] - 1;

        checkCuestErrors (cuestAOShellCreate (
            handle, 0, get_L (quick_basis.ktype[i]), quick_basis.kprim[i],
            aexp[ifirst], dcoeff[ifirst], aoshell_params, &shells[i]));
    }

    checkCuestErrors (
        cuestParametersDestroy (CUEST_AOSHELL_PARAMETERS, aoshell_params));

    // ============ //
    // set up basis //
    // ============ //

    cuestAOBasis_t           basis;
    cuestAOBasisParameters_t basis_params;
    checkCuestErrors (
        cuestParametersCreate (CUEST_AOBASIS_PARAMETERS, &basis_params));

    cuestWorkspaceDescriptor_t *persistWD
        = malloc (sizeof (cuestWorkspaceDescriptor_t));
    cuestWorkspaceDescriptor_t *tmpWD
        = malloc (sizeof (cuestWorkspaceDescriptor_t));

    checkCuestErrors (cuestAOBasisCreateWorkspaceQuery (
        handle, natom, nshells_per_atom, shells, basis_params, persistWD,
        tmpWD, &basis));

    cuestWorkspace_t *persist_wksp = allocateWorkspace (persistWD);
    cuestWorkspace_t *tmp_wksp     = allocateWorkspace (tmpWD);

    free (persistWD);
    free (tmpWD);

    checkCuestErrors (cuestAOBasisCreate (handle, natom, nshells_per_atom,
                                          shells, basis_params, persist_wksp,
                                          tmp_wksp, &basis));

    freeWorkspace (tmp_wksp);
    checkCuestErrors (
        cuestParametersDestroy (CUEST_AOBASIS_PARAMETERS, basis_params));

    for (size_t i = 0; i < nshell; ++i)
        checkCuestErrors (cuestAOShellDestroy (shells[i]));

    free (shells);

    // ================= //
    // query information //
    // ================= //

    uint64_t query_natom      = 0;
    uint64_t query_nshell     = 0;
    uint64_t query_nao        = 0;
    uint64_t query_ncart      = 0;
    uint64_t query_nprimitive = 0;
    uint64_t query_max_L      = 0;
    int32_t  query_is_pure    = 0;

    checkCuestErrors (cuestQuery (handle, CUEST_AOBASIS, basis,
                                  CUEST_AOBASIS_NUM_ATOM, &query_natom,
                                  sizeof (uint64_t)));
    checkCuestErrors (cuestQuery (handle, CUEST_AOBASIS, basis,
                                  CUEST_AOBASIS_NUM_SHELL, &query_nshell,
                                  sizeof (uint64_t)));
    checkCuestErrors (cuestQuery (handle, CUEST_AOBASIS, basis,
                                  CUEST_AOBASIS_NUM_AO, &query_nao,
                                  sizeof (uint64_t)));
    checkCuestErrors (cuestQuery (handle, CUEST_AOBASIS, basis,
                                  CUEST_AOBASIS_NUM_CART, &query_ncart,
                                  sizeof (uint64_t)));
    checkCuestErrors (cuestQuery (handle, CUEST_AOBASIS, basis,
                                  CUEST_AOBASIS_NUM_PRIMITIVE,
                                  &query_nprimitive, sizeof (uint64_t)));
    checkCuestErrors (cuestQuery (handle, CUEST_AOBASIS, basis,
                                  CUEST_AOBASIS_MAX_L, &query_max_L,
                                  sizeof (uint64_t)));
    checkCuestErrors (cuestQuery (handle, CUEST_AOBASIS, basis,
                                  CUEST_AOBASIS_IS_PURE, &query_is_pure,
                                  sizeof (int32_t)));

    printf ("AO Basis from handle:\n");
    printf ("%-10s = %6llu\n", "natom", query_natom);
    printf ("%-10s = %6llu\n", "nshell", query_nshell);
    printf ("%-10s = %6llu\n", "nao", query_nao);
    printf ("%-10s = %6llu\n", "ncart", query_ncart);
    printf ("%-10s = %6llu\n", "nprimitive", query_nprimitive);
    printf ("%-10s = %6llu\n", "max_L", query_max_L);
    printf ("%-10s = %6s\n", "is_pure", query_is_pure ? "true" : "false");

    // ======== //
    // clean up //
    // ======== //

    checkCuestErrors (cuestAOBasisDestroy (basis));
    freeWorkspace (persist_wksp);
    checkCuestErrors (cuestDestroy (handle));

    return 0;
}
