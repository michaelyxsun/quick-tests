#include <assert.h>
#include <stdatomic.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef LOCAL
#include "/Users/msun/rehs2026/cuest/libcuest-linux-sbsa-0.1.1.1_cuda13-archive/include/cuest.h"
#else
#include <cuda_runtime.h>
#include <cuest.h>
#endif

#include "constants.h"
#include "helper_status.h"
#include "helper_workspace.h"
#include "molspec.h"
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

    cuestWorkspace_t *persistBasisWorkspace = allocateWorkspace (persistWD);
    cuestWorkspace_t *tmpBasisWorkspace     = allocateWorkspace (tmpWD);

    checkCuestErrors (cuestAOBasisCreate (
        handle, natom, nshells_per_atom, shells, basis_params,
        persistBasisWorkspace, tmpBasisWorkspace, &basis));

    freeWorkspace (tmpBasisWorkspace);
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

    // ================ //
    // set up pair list //
    // ================ //

    assert (natom == 3);
    double *xyz_flat = malloc (natom * 3 * sizeof (double));

    for (size_t i = 0; i < natom; ++i) {
        size_t i3        = 3 * i;
        xyz_flat[i3]     = xyz[i][0];
        xyz_flat[i3 + 1] = xyz[i][1];
        xyz_flat[i3 + 2] = xyz[i][2];
    }

    cuestAOPairList_t           pair_list;
    cuestAOPairListParameters_t pair_list_params;
    checkCuestErrors (cuestParametersCreate (CUEST_AOPAIRLIST_PARAMETERS,
                                             &pair_list_params));
    checkCuestErrors (cuestAOPairListCreateWorkspaceQuery (
        handle, basis, natom, xyz_flat, 1e-14, pair_list_params, persistWD,
        tmpWD, &pair_list));

    cuestWorkspace_t *persistAOPairListWorkspace
        = allocateWorkspace (persistWD);
    cuestWorkspace_t *tmpAOPairListWorkspace = allocateWorkspace (tmpWD);

    checkCuestErrors (cuestAOPairListCreate (
        handle, basis, natom, xyz_flat, 1e-14, pair_list_params,
        persistAOPairListWorkspace, tmpAOPairListWorkspace, &pair_list));
    checkCuestErrors (cuestParametersDestroy (CUEST_AOPAIRLIST_PARAMETERS,
                                              pair_list_params));
    freeWorkspace (tmpAOPairListWorkspace);
    free (xyz_flat);

    // ========================= //
    // one-electron integral plan //
    // ========================= //

    cuestOEIntPlan_t           oeint_plan;
    cuestOEIntPlanParameters_t oeint_plan_params;
    checkCuestErrors (cuestParametersCreate (CUEST_OEINTPLAN_PARAMETERS,
                                             &oeint_plan_params));
    checkCuestErrors (cuestOEIntPlanCreateWorkspaceQuery (
        handle, basis, pair_list, oeint_plan_params, persistWD, tmpWD,
        &oeint_plan));

    cuestWorkspace_t *persistOEIntPlanWorkspace
        = allocateWorkspace (persistWD);
    cuestWorkspace_t *tmpOEIntPlanWorkspace = allocateWorkspace (tmpWD);
    checkCuestErrors (cuestOEIntPlanCreate (
        handle, basis, pair_list, oeint_plan_params, persistOEIntPlanWorkspace,
        tmpOEIntPlanWorkspace, &oeint_plan));

    checkCuestErrors (cuestParametersDestroy (CUEST_OEINTPLAN_PARAMETERS,
                                              oeint_plan_params));
    freeWorkspace (tmpOEIntPlanWorkspace);

    // ============================== //
    // compute one-electron integrals //
    // ============================== //

    uint64_t nao = 0;
    checkCuestErrors (cuestQuery (handle, CUEST_AOBASIS, basis,
                                  CUEST_AOBASIS_NUM_AO, &nao,
                                  sizeof (uint64_t)));

    double *d_S;
    size_t  d_S_siz = nao * nao * sizeof (double);
    if (cudaMalloc ((void **)&d_S, d_S_siz)) {
        fprintf (stderr, "Failed to allocate device buffer\n");
        goto persist_free;
    }

    cuestOverlapComputeParameters_t overlap_compute_params;
    checkCuestErrors (cuestParametersCreate (CUEST_OVERLAPCOMPUTE_PARAMETERS,
                                             &overlap_compute_params));
    checkCuestErrors (cuestOverlapComputeWorkspaceQuery (
        handle, oeint_plan, overlap_compute_params, tmpWD, d_S));

    cuestWorkspace_t *tmpSWorkspace = allocateWorkspace (tmpWD);
    checkCuestErrors (cuestOverlapCompute (
        handle, oeint_plan, overlap_compute_params, tmpSWorkspace, d_S));

    freeWorkspace (tmpSWorkspace);
    checkCuestErrors (cuestParametersDestroy (CUEST_OVERLAPCOMPUTE_PARAMETERS,
                                              overlap_compute_params));

    // ==================== //
    // print overlap matrix //
    // ==================== //

    double *buf = malloc (nao * nao * sizeof (double));
    if (!buf) {
        fprintf (stderr, "malloc buf failed\n");
        goto persist_free;
    }

    cudaMemcpy (buf, d_S, d_S_siz, cudaMemcpyDeviceToHost);

    printf ("-------- S --------");
    for (int i = 0; i < nao; ++i) {
        for (int j = 0; j < nao; ++j)
            printf ("%16.10f", buf[i * nao + j]);
        putchar ('\n');
    }
    printf ("------ END S ------");

    free (buf);

    if (cudaFree (d_S) != cudaSuccess) {
        fprintf (stderr, "cudaFree failed\n");
        goto persist_free;
    }

    // ========================= //
    // clean up persistent stuff //
    // ========================= //

persist_free:

    free (persistWD);
    free (tmpWD);

    checkCuestErrors (cuestOEIntPlanDestroy (oeint_plan));
    freeWorkspace (persistOEIntPlanWorkspace);

    checkCuestErrors (cuestAOPairListDestroy (pair_list));
    freeWorkspace (persistAOPairListWorkspace);

    checkCuestErrors (cuestAOBasisDestroy (basis));
    freeWorkspace (persistBasisWorkspace);

    checkCuestErrors (cuestDestroy (handle));

    return 0;
}
