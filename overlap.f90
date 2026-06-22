program overlap
    use, intrinsic::iso_c_binding, only: c_int64_t, c_double
    implicit none
    
    interface
        subroutine cuest_init_basis(natom, nshell, ncenter, first_basis_function, &
                                    last_basis_function, katom, ktype, kprim, gcexpo, gccoeff, xyz) &
                                    bind(c, name="cuest_init_basis")
            use, intrinsic::iso_c_binding, only: c_int64_t, c_double
            implicit none
            integer(c_int64_t), intent(in), value :: natom
            integer(c_int64_t), intent(in), value :: nshell
            integer(c_int64_t), intent(in) :: ncenter(*)
            integer(c_int64_t), intent(in) :: first_basis_function(*)
            integer(c_int64_t), intent(in) :: last_basis_function(*)
            integer(c_int64_t), intent(in) :: katom(*)
            integer(c_int64_t), intent(in) :: ktype(*)
            integer(c_int64_t), intent(in) :: kprim(*)
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
    double precision :: gcexpo_(3,7)
    double precision :: gccoeff_(3,7)
    double precision :: xyz_(3, 3)

    gcexpo_(   1,   1)=    0.1307093214E+03
    gcexpo_(   2,   1)=    0.2380886605E+02
    gcexpo_(   3,   1)=    0.6443608313E+01
    gcexpo_(   1,   2)=    0.5033151319E+01
    gcexpo_(   2,   2)=    0.1169596125E+01
    gcexpo_(   3,   2)=    0.3803889600E+00
    gcexpo_(   1,   3)=    0.5033151319E+01
    gcexpo_(   2,   3)=    0.1169596125E+01
    gcexpo_(   3,   3)=    0.3803889600E+00
    gcexpo_(   1,   4)=    0.5033151319E+01
    gcexpo_(   2,   4)=    0.1169596125E+01
    gcexpo_(   3,   4)=    0.3803889600E+00
    gcexpo_(   1,   5)=    0.5033151319E+01
    gcexpo_(   2,   5)=    0.1169596125E+01
    gcexpo_(   3,   5)=    0.3803889600E+00
    gcexpo_(   1,   6)=    0.3425250914E+01
    gcexpo_(   2,   6)=    0.6239137298E+00
    gcexpo_(   3,   6)=    0.1688554040E+00
    gcexpo_(   1,   7)=    0.3425250914E+01
    gcexpo_(   2,   7)=    0.6239137298E+00
    gcexpo_(   3,   7)=    0.1688554040E+00

    gccoeff_(   1,   1)=    0.4251943278E+01
    gccoeff_(   2,   1)=    0.4112294424E+01
    gccoeff_(   3,   1)=    0.1281622551E+01
    gccoeff_(   1,   2)=   -0.2394130049E+00
    gccoeff_(   2,   2)=    0.3202342354E+00
    gccoeff_(   3,   2)=    0.2416855546E+00
    gccoeff_(   1,   3)=    0.1675450196E+01
    gccoeff_(   2,   3)=    0.1053568044E+01
    gccoeff_(   3,   3)=    0.1669028791E+00
    gccoeff_(   1,   4)=    0.1675450196E+01
    gccoeff_(   2,   4)=    0.1053568044E+01
    gccoeff_(   3,   4)=    0.1669028791E+00
    gccoeff_(   1,   5)=    0.1675450196E+01
    gccoeff_(   2,   5)=    0.1053568044E+01
    gccoeff_(   3,   5)=    0.1669028791E+00
    gccoeff_(   1,   6)=    0.2769343551E+00
    gccoeff_(   2,   6)=    0.2678388516E+00
    gccoeff_(   3,   6)=    0.8347367113E-01
    gccoeff_(   1,   7)=    0.2769343551E+00
    gccoeff_(   2,   7)=    0.2678388516E+00
    gccoeff_(   3,   7)=    0.8347367113E-01

    xyz_(   1,  1)= -0.6395
    xyz_(   2,  1)=  0.0072
    xyz_(   3,  1)=  0.4521
    xyz_(   1,  2)= -0.6332
    xyz_(   2,  2)= -0.0036
    xyz_(   3,  2)= -1.5737
    xyz_(   1,  3)=  1.2727
    xyz_(   2,  3)= -0.0036
    xyz_(   3,  3)=  1.1216

    print *, "hi from fortran"

    call cuest_init_basis(int(natom_, c_int64_t), int(nshell_, c_int64_t), int(ncenter_, c_int64_t), &
                          int(first_basis_function_, c_int64_t), int(last_basis_function_, c_int64_t), &
                          int(katom_, c_int64_t), int(ktype_, c_int64_t), int(kprim_, c_int64_t), &
                          real(gcexpo_, c_double), real(gccoeff_, c_double), real(xyz_, c_double))

    print *, "bye from fortran"
end program overlap
