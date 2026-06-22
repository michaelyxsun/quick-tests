program overlap
    implicit none
    
    interface
        subroutine cuest_init_basis(natom, nshell, ncenter, first_basis_function, &
                                    last_basis_function, katom, ktype, kprim, gcexpo, gccoeff, xyz) &
                                    bind(c, name="cuest_init_basis")
            use, intrinsic::iso_c_binding, only: c_double
            implicit none
            integer, intent(in), value :: natom
            integer, intent(in), value :: nshell
            integer, intent(in) :: ncenter(*)
            integer, intent(in) :: first_basis_function(*)
            integer, intent(in) :: last_basis_function(*)
            integer, intent(in) :: katom(*)
            integer, intent(in) :: ktype(*)
            integer, intent(in) :: kprim(*)
            real(kind=c_double), intent(in) :: gcexpo(*)
            real(kind=c_double), intent(in) :: gccoeff(*)
            real(kind=c_double), intent(in) :: xyz(*)
        end subroutine
    end interface

    integer :: natom_ = 3
    integer :: nshell_ = 4
    integer :: ncenter_(7) = [ 1, 1, 1, 1, 1, 2, 3 ]
    integer :: first_basis_function_(3) = [ 1, 6, 7 ]
    integer :: last_basis_function_(3) = [ 5, 6, 7 ]
    integer :: katom_(4) = [ 1, 1, 2, 3 ]
    integer :: ktype_(4) = [ 1, 4, 1, 1 ]
    integer :: kprim_(4) = [ 3, 3, 3, 3 ]
    double precision :: gcexpo_(7,3) = reshape([ &
    0.130709321400D+03, 0.503315131900D+01, 0.503315131900D+01, &
    0.503315131900D+01, 0.503315131900D+01, 0.342525091400D+01, &
    0.342525091400D+01, &
    0.238088660500D+02, 0.116959612500D+01, 0.116959612500D+01, &
    0.116959612500D+01, 0.116959612500D+01, 0.623913729800D+00, &
    0.623913729800D+00, &
    0.644360831300D+01, 0.380388960000D+00, 0.380388960000D+00, &
    0.380388960000D+00, 0.380388960000D+00, 0.168855404000D+00, &
    0.168855404000D+00 ], &
    [7,3] )
    double precision :: gccoeff_(7,3) = reshape([ &
    0.4251943278D+01, -0.2394130049D+00, 0.1675450196D+01, &
    0.1675450196D+01, 0.1675450196D+01, 0.2769343551D+00, &
    0.2769343551D+00, &
    0.4112294424D+01, 0.3202342354D+00, 0.1053568044D+01, &
    0.1053568044D+01, 0.1053568044D+01, 0.2678388516D+00, &
    0.2678388516D+00, &
    0.1281622551D+01, 0.2416855546D+00, 0.1669028791D+00, &
    0.1669028791D+00, 0.1669028791D+00, 0.8347367113D-01, &
    0.8347367113D-01 ], &
    [7,3] )
    double precision :: xyz_(2, 3) = reshape([ -0.6395, -0.6333,1.2727,0.0072,-0.0036,-0.0036,0.4520,-1.5737, 1.1216 ], [2,3])

    print *, "hi from fortran"

    call cuest_init_basis(natom_, nshell_, ncenter_, first_basis_function_, last_basis_function_, &
                     katom_, ktype_, kprim_, gcexpo_, gccoeff_, xyz_)

    print *, "bye from fortran"
end program overlap
