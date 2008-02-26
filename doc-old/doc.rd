=begin
= Summary
== Class Tree
  * ((<GSL>))
    * ((<Blas>))
    * ((<Blas_complex>))
    * ((<Block>))
    * ((<CanPermutation>))
    * ((<Chebyshev>))
    * ((<Combination>))
    * ((<Complex>))
    * ((<Const>))
      * ((<CGS>))
      * ((<MKS>))
      * ((<Num>))
    * ((<DHT>))
    * ((<Diff>))
    * ((<Eigen>))
    * ((<Eigen_complex>))
    * ((<Fit>))
    * ((<Function>))
    * ((<GSLException>))
    * ((<Histogram>))
      * ((<Hist>))
      * ((<Hist2D>))
      * ((<Hist2D_PDF>))
      * ((<Hist_PDF>))
    * ((<Integration>))
      * ((<QAWOTable>))
      * ((<QAWSTable>))
    * ((<Interpolation>))
      * ((<Accel>))
      * ((<Interp>))
      * ((<Spline>))
    * ((<Linalg>))
    * ((<MFunction>))
    * ((<Matrix>))
    * ((<Matrix_complex>))
    * ((<Minimizer>))
    * ((<MonteCarlo>))
      * ((<Function>))
      * ((<Miser>))
      * ((<Plain>))
      * ((<Vegas>))
    * ((<Multiroot>))
      * ((<MFDFSolver>))
      * ((<MFSolver>))
    * ((<Permutation>))
    * ((<Poly>))
      * ((<DD>))
      * ((<Workspace>))
    * ((<Random>))
      * ((<Discrete>))
      * ((<QRNG>))
      * ((<RND>))
      * ((<RNG>))
    * ((<Result>))
    * ((<Result10>))
    * ((<Siman>))
    * ((<Solver>))
      * ((<FDFSolver>))
      * ((<FSolver>))
    * ((<Special>))
      * ((<Airy>))
      * ((<Bessel>))
      * ((<Clausen>))
      * ((<Conical>))
      * ((<Coulomb>))
      * ((<Coupling>))
      * ((<Dawson>))
      * ((<Debye>))
      * ((<Dilog>))
      * ((<Elementary>))
      * ((<EllInt>))
      * ((<EllJac>))
      * ((<Error>))
      * ((<Exp>))
      * ((<ExpInt>))
      * ((<FermiDirac>))
      * ((<Gamma>))
      * ((<Gegenbauer>))
      * ((<Hyperg>))
      * ((<Laguerre>))
      * ((<Lambert>))
      * ((<Legendre>))
      * ((<Log>))
      * ((<Psi>))
      * ((<Synchrotron>))
      * ((<Transport>))
      * ((<Trig>))
      * ((<Zeta>))
    * ((<Stats>))
    * ((<Sum>))
      * ((<Levin>))
      * ((<LevinTrunc>))
    * ((<Vector>))
    * ((<Vector_complex>))
= Classes & Modules
== ((:GSL:))
=== Module Functions
--- GSL#strerror (1)
--- GSL#set_handler (1)
=== Constants
--- ERR_INTERN
--- ERR_OFF
--- ERR_RUBY
--- GSL_CONTINUE
--- GSL_EBADFUNC
--- GSL_EBADLEN
--- GSL_EBADTOL
--- GSL_ECACHE
--- GSL_EDIVERGE
--- GSL_EDOM
--- GSL_EFACTOR
--- GSL_EFAILED
--- GSL_EFAULT
--- GSL_EINVAL
--- GSL_ELOSS
--- GSL_EMAXITER
--- GSL_ENOMEM
--- GSL_ENOPROG
--- GSL_ENOPROGJ
--- GSL_ENOTSQR
--- GSL_EOF
--- GSL_EOVRFLW
--- GSL_ERANGE
--- GSL_EROUND
--- GSL_ERUNAWAY
--- GSL_ESANITY
--- GSL_ESING
--- GSL_ETABLE
--- GSL_ETOL
--- GSL_ETOLF
--- GSL_ETOLG
--- GSL_ETOLX
--- GSL_EUNDRFLW
--- GSL_EUNIMPL
--- GSL_EUNSUP
--- GSL_EZERODIV
--- GSL_FAILURE
--- GSL_SUCCESS
--- RELEASE
--- VERSION
--- VERSION_MAJOR
--- VERSION_MINOR
== ((:Blas:))
=== Module Functions
--- Blas#dswap (2)
--- Blas#drotg (2)
--- Blas#drotmg (4)
=== Constants
--- ConjTrans
--- Left
--- Lower
--- NoTrans
--- NonUnit
--- Right
--- Trans
--- Unit
--- Upper
== ((:Blas_complex:))
=== Module Functions
--- Blas_complex#zswap (2)
=== Constants
--- ConjTrans
--- Left
--- Lower
--- NoTrans
--- NonUnit
--- Right
--- Trans
--- Unit
--- Upper
== ((:Block:))
=== Superclass
(({Object}))
=== Class Methods
--- Block.new (1)
--- Block.alloc (1)
--- Block.calloc (1)
=== Instance Methods
--- Block#size (0)
--- Block#fwrite (1)
--- Block#fread (1)
--- Block#fprintf (2)
--- Block#fscanf (1)
== ((:CanPermutation:))
=== Superclass
((<Permutation>))
=== Class Methods
--- CanPermutation.new (1)
=== Instance Methods
--- CanPermutation#to_canonical (0)
--- CanPermutation#to_linear (0)
--- CanPermutation#equal (1)
--- CanPermutation#mul (1)
--- CanPermutation#reverse! (0)
--- CanPermutation#cycles (0)
--- CanPermutation#reset (0)
== ((:Chebyshev:))
=== Superclass
(({Object}))
=== Class Methods
--- Chebyshev.new (4)
=== Instance Methods
--- Chebyshev#initialize (4)
--- Chebyshev#eval (1)
--- Chebyshev#eval_err (1)
--- Chebyshev#eval_n (2)
--- Chebyshev#eval_n_err (2)
--- Chebyshev#eval_mode (2)
--- Chebyshev#eval_mode_e (2)
--- Chebyshev#calc_deriv (1)
--- Chebyshev#calc_integ (1)
== ((:Combination:))
=== Superclass
(({Object}))
=== Class Methods
--- Combination.new (2)
--- Combination.new_ary (2)
=== Instance Methods
--- Combination#initialize (2)
--- Combination#init_first (0)
--- Combination#init_last (0)
--- Combination#get (1)
--- Combination#n (0)
--- Combination#k (0)
--- Combination#data (0)
--- Combination#valid (0)
--- Combination#== (1)
--- Combination#_dump_data (0)
--- Combination#_load_data (1)
--- Combination#next (0)
--- Combination#prev (0)
--- Combination#each (0)
--- Combination#reverse_each (0)
--- Combination#fwrite (1)
--- Combination#fread (1)
--- Combination#fprintf (2)
--- Combination#fscanf (1)
== ((:Complex:))
=== Superclass
(({Object}))
=== Class Methods
--- Complex.new (1)
--- Complex.new2 (2)
--- Complex.new3 (2)
--- Complex.float_equal (3)
--- Complex.sqrt (1)
--- Complex.sqrt_real (1)
--- Complex.exp (1)
--- Complex.pow (2)
--- Complex.pow_real (2)
--- Complex.log (1)
--- Complex.log10 (1)
--- Complex.logb (2)
--- Complex.sin (1)
--- Complex.cos (1)
--- Complex.tan (1)
--- Complex.csc (1)
--- Complex.sec (1)
--- Complex.cot (1)
--- Complex.sin_e (1)
--- Complex.cos_e (1)
--- Complex.logsin_e (1)
--- Complex.arcsin (1)
--- Complex.arcsin_real (1)
--- Complex.arccos (1)
--- Complex.arccos_real (1)
--- Complex.arctan (1)
--- Complex.arccsc (1)
--- Complex.arccsc_real (1)
--- Complex.arcsec (1)
--- Complex.arcsec_real (1)
--- Complex.arccot (1)
--- Complex.sinh (1)
--- Complex.cosh (1)
--- Complex.tanh (1)
--- Complex.csch (1)
--- Complex.sech (1)
--- Complex.coth (1)
--- Complex.arcsinh (1)
--- Complex.arccosh (1)
--- Complex.arccosh_real (1)
--- Complex.arctanh (1)
--- Complex.arctanh_real (1)
--- Complex.arccsch (1)
--- Complex.arcsech (1)
--- Complex.arccoth (1)
=== Instance Methods
--- Complex#initialize (2)
--- Complex#== (1)
--- Complex#to_s (0)
--- Complex#real (0)
--- Complex#image (0)
--- Complex#arg (0)
--- Complex#abs (0)
--- Complex#abs2 (0)
--- Complex#logabs (0)
--- Complex#add (1)
--- Complex#add_real (1)
--- Complex#add_imag (1)
--- Complex#sub (1)
--- Complex#sub_real (1)
--- Complex#sub_imag (1)
--- Complex#mul (1)
--- Complex#mul_real (1)
--- Complex#mul_imag (1)
--- Complex#div (1)
--- Complex#div_real (1)
--- Complex#div_imag (1)
--- Complex#conjugate (0)
--- Complex#inverse (0)
--- Complex#negative (0)
--- Complex#to_a (0)
--- Complex#_dump_data (0)
--- Complex#_load_data (1)
== ((:Const:))
== ((:CGS:))
=== Constants
--- ACRE
--- ANGSTROM
--- ASTRONOMICAL_UNIT
--- BAR
--- BARN
--- BOHR_MAGNETON
--- BOHR_RADIUS
--- BOLTZMANN
--- BTU
--- CALORIE
--- CANADIAN_GALLON
--- CARAT
--- CUP
--- CURIE
--- DAY
--- DYNE
--- ELECTRON_CHARGE
--- ELECTRON_MAGNETIC_MOMENT
--- ELECTRON_VOLT
--- ERG
--- FARADAY
--- FATHOM
--- FLUID_OUNCE
--- FOOT
--- FOOTCANDLE
--- FOOTLAMBERT
--- GAUSS
--- GRAM_FORCE
--- GRAVITATIONAL_CONSTANT
--- GRAV_ACCEL
--- HECTARE
--- HORSEPOWER
--- HOUR
--- INCH
--- INCH_OF_MERCURY
--- INCH_OF_WATER
--- JOULE
--- KILOMETERS_PER_HOUR
--- KILOPOUND_FORCE
--- KNOT
--- LAMBERT
--- LIGHT_YEAR
--- LITER
--- LUMEN
--- LUX
--- MASS_ELECTRON
--- MASS_MUON
--- MASS_NEUTRON
--- MASS_PROTON
--- METER_OF_MERCURY
--- METRIC_TON
--- MICRON
--- MIL
--- MILE
--- MILES_PER_HOUR
--- MINUTE
--- MOLAR_GAS
--- NAUTICAL_MILE
--- NEWTON
--- NUCLEAR_MAGNETON
--- OUNCE_MASS
--- PARSEC
--- PHOT
--- PINT
--- PLANCKS_CONSTANT_H
--- PLANCKS_CONSTANT_HBAR
--- POINT
--- POISE
--- POUNDAL
--- POUND_FORCE
--- POUND_MASS
--- PROTON_MAGNETIC_MOMENT
--- PSI
--- QUART
--- RAD
--- ROENTGEN
--- RYDBERG
--- SOLAR_MASS
--- SPEED_OF_LIGHT
--- STANDARD_GAS_VOLUME
--- STD_ATMOSPHERE
--- STILB
--- STOKES
--- TABLESPOON
--- TEASPOON
--- TEXPOINT
--- THERM
--- TON
--- TORR
--- TROY_OUNCE
--- UK_GALLON
--- UK_TON
--- UNIFIED_ATOMIC_MASS
--- US_GALLON
--- VACUUM_PERMEABILITY
--- VACUUM_PERMITTIVITY
--- WEEK
--- YARD
== ((:MKS:))
=== Constants
--- ACRE
--- ANGSTROM
--- ASTRONOMICAL_UNIT
--- BAR
--- BARN
--- BOHR_MAGNETON
--- BOHR_RADIUS
--- BOLTZMANN
--- BTU
--- CALORIE
--- CANADIAN_GALLON
--- CARAT
--- CUP
--- CURIE
--- DAY
--- DYNE
--- ELECTRON_CHARGE
--- ELECTRON_MAGNETIC_MOMENT
--- ELECTRON_VOLT
--- ERG
--- FARADAY
--- FATHOM
--- FLUID_OUNCE
--- FOOT
--- FOOTCANDLE
--- FOOTLAMBERT
--- GAUSS
--- GRAM_FORCE
--- GRAVITATIONAL_CONSTANT
--- GRAV_ACCEL
--- HECTARE
--- HORSEPOWER
--- HOUR
--- INCH
--- INCH_OF_MERCURY
--- INCH_OF_WATER
--- JOULE
--- KILOMETERS_PER_HOUR
--- KILOPOUND_FORCE
--- KNOT
--- LAMBERT
--- LIGHT_YEAR
--- LITER
--- LUMEN
--- LUX
--- MASS_ELECTRON
--- MASS_MUON
--- MASS_NEUTRON
--- MASS_PROTON
--- METER_OF_MERCURY
--- METRIC_TON
--- MICRON
--- MIL
--- MILE
--- MILES_PER_HOUR
--- MINUTE
--- MOLAR_GAS
--- NAUTICAL_MILE
--- NEWTON
--- NUCLEAR_MAGNETON
--- OUNCE_MASS
--- PARSEC
--- PHOT
--- PINT
--- PLANCKS_CONSTANT_H
--- PLANCKS_CONSTANT_HBAR
--- POINT
--- POISE
--- POUNDAL
--- POUND_FORCE
--- POUND_MASS
--- PROTON_MAGNETIC_MOMENT
--- PSI
--- QUART
--- RAD
--- ROENTGEN
--- RYDBERG
--- SOLAR_MASS
--- SPEED_OF_LIGHT
--- STANDARD_GAS_VOLUME
--- STD_ATMOSPHERE
--- STILB
--- STOKES
--- TABLESPOON
--- TEASPOON
--- TEXPOINT
--- THERM
--- TON
--- TORR
--- TROY_OUNCE
--- UK_GALLON
--- UK_TON
--- UNIFIED_ATOMIC_MASS
--- US_GALLON
--- VACUUM_PERMEABILITY
--- VACUUM_PERMITTIVITY
--- WEEK
--- YARD
== ((:Num:))
=== Constants
--- ATTO
--- AVOGADRO
--- EXA
--- FEMTO
--- FINE_STRUCTURE
--- GIGA
--- KILO
--- MEGA
--- MICRO
--- MILLI
--- NANO
--- PETA
--- PICO
--- TERA
--- YOCTO
--- YOTTA
--- ZEPTO
--- ZETTA
== ((:DHT:))
=== Superclass
(({Object}))
=== Class Methods
--- DHT.new (3)
=== Instance Methods
--- DHT#initialize (3)
--- DHT#k_sample (1)
--- DHT#x_sample (1)
--- DHT#apply (2)
== ((:Diff:))
=== Module Functions
--- Diff#central (2)
--- Diff#forward (2)
--- Diff#backward (2)
== ((:Eigen:))
=== Module Functions
--- Eigen#symmv_sort (3)
=== Constants
--- SORT_ABS_ASC
--- SORT_ABS_DESC
--- SORT_VAL_ASC
--- SORT_VAL_DESC
== ((:Eigen_complex:))
=== Module Functions
--- Eigen_complex#hermv_sort (3)
=== Constants
--- SORT_ABS_ASC
--- SORT_ABS_DESC
--- SORT_VAL_ASC
--- SORT_VAL_DESC
== ((:Fit:))
=== Module Functions
--- Fit#linear (2)
--- Fit#wlinear (3)
--- Fit#linear_est (6)
--- Fit#mul (2)
--- Fit#wmul (3)
--- Fit#mul_est (6)
== ((:Function:))
=== Superclass
(({Object}))
=== Class Methods
--- Function.new (0)
--- Function.new2 (1)
--- Function.alloc (0)
=== Instance Methods
--- Function#eval (1)
--- Function#arity (0)
--- Function#proc (0)
--- Function#set (0)
--- Function#integration_qng (4)
--- Function#integration_qag (6)
--- Function#integration_qags (5)
--- Function#integration_qagp (4)
--- Function#integration_qagi (3)
--- Function#integration_qagiu (4)
--- Function#integration_qagil (4)
--- Function#integration_qawc (6)
--- Function#integration_qaws (6)
--- Function#integration_qawo (5)
--- Function#integration_qawf (4)
== ((:GSLException:))
=== Superclass
(({Exception}))
== ((:Histogram:))
== ((:Hist:))
=== Superclass
(({Object}))
=== Class Methods
--- Hist.new (1)
--- Hist.new2 (3)
=== Instance Methods
--- Hist#initialize (0)
--- Hist#set_range (2)
--- Hist#set_ranges (1)
--- Hist#set_ranges_uniform (2)
--- Hist#clone (0)
--- Hist#increment (1)
--- Hist#accumulate (2)
--- Hist#get (1)
--- Hist#get_range (1)
--- Hist#each (0)
--- Hist#each_bin (0)
--- Hist#max (0)
--- Hist#min (0)
--- Hist#bins (0)
--- Hist#reset (0)
--- Hist#find (1)
--- Hist#max_val (0)
--- Hist#max_bin (0)
--- Hist#min_val (0)
--- Hist#min_bin (0)
--- Hist#mean (0)
--- Hist#sigma (0)
--- Hist#sum (0)
--- Hist#equal_bins_p (1)
--- Hist#add (1)
--- Hist#sub (1)
--- Hist#mul (1)
--- Hist#div (1)
--- Hist#scale (1)
--- Hist#shift (1)
--- Hist#fwrite (1)
--- Hist#fread (1)
--- Hist#fprintf (3)
--- Hist#dump (0)
--- Hist#fscanf (1)
=== Module Functions
--- Hist#memcpy (2)
== ((:Hist2D:))
=== Superclass
(({Object}))
=== Class Methods
--- Hist2D.new (2)
--- Hist2D.new2 (6)
=== Instance Methods
--- Hist2D#initialize (0)
--- Hist2D#set_xrange (2)
--- Hist2D#set_yrange (2)
--- Hist2D#set_ranges (2)
--- Hist2D#set_ranges_uniform (4)
--- Hist2D#clone (0)
--- Hist2D#increment (2)
--- Hist2D#accumulate (3)
--- Hist2D#get (2)
--- Hist2D#get_xrange (1)
--- Hist2D#get_yrange (1)
--- Hist2D#xmax (0)
--- Hist2D#xmin (0)
--- Hist2D#nx (0)
--- Hist2D#ymax (0)
--- Hist2D#ymin (0)
--- Hist2D#ny (0)
--- Hist2D#reset (0)
--- Hist2D#find (2)
--- Hist2D#max_val (0)
--- Hist2D#min_val (0)
--- Hist2D#max_bin (0)
--- Hist2D#min_bin (0)
--- Hist2D#xmean (0)
--- Hist2D#ymean (0)
--- Hist2D#xsigma (0)
--- Hist2D#ysigma (0)
--- Hist2D#cov (0)
--- Hist2D#sum (0)
--- Hist2D#equal_bins_p (1)
--- Hist2D#add (1)
--- Hist2D#sub (1)
--- Hist2D#mul (1)
--- Hist2D#div (1)
--- Hist2D#scale (1)
--- Hist2D#shift (1)
--- Hist2D#fwrite (1)
--- Hist2D#fread (1)
--- Hist2D#fprintf (3)
--- Hist2D#dump (0)
--- Hist2D#fscanf (1)
=== Module Functions
--- Hist2D#memcpy (2)
== ((:Hist2D_PDF:))
=== Superclass
(({Object}))
=== Class Methods
--- Hist2D_PDF.new (1)
=== Instance Methods
--- Hist2D_PDF#initialize (0)
--- Hist2D_PDF#sample (2)
== ((:Hist_PDF:))
=== Superclass
(({Object}))
=== Class Methods
--- Hist_PDF.new (1)
=== Instance Methods
--- Hist_PDF#initialize (0)
--- Hist_PDF#sample (1)
== ((:Integration:))
=== Constants
--- COSINE
--- GAUSS15
--- GAUSS21
--- GAUSS31
--- GAUSS41
--- GAUSS51
--- GAUSS61
--- SINE
== ((:QAWOTable:))
=== Superclass
(({Object}))
=== Class Methods
--- QAWOTable.alloc (4)
--- QAWOTable.new (4)
=== Instance Methods
--- QAWOTable#to_a (0)
--- QAWOTable#set (-1)
--- QAWOTable#set_length (1)
== ((:QAWSTable:))
=== Superclass
(({Object}))
=== Class Methods
--- QAWSTable.alloc (4)
--- QAWSTable.new (4)
=== Instance Methods
--- QAWSTable#to_a (0)
--- QAWSTable#set (-1)
== ((:Interpolation:))
=== Constants
--- AKIMA
--- AKIMA_PERIODIC
--- CSPLINE
--- CSPLINE_PERIODIC
--- LINEAR
--- POLYNOMIAL
== ((:Accel:))
=== Superclass
(({Object}))
=== Class Methods
--- Accel.new (0)
=== Instance Methods
--- Accel#initialize (0)
--- Accel#reset (0)
--- Accel#bsearch (4)
--- Accel#find (2)
== ((:Interp:))
=== Superclass
(({Object}))
=== Class Methods
--- Interp.new (2)
=== Instance Methods
--- Interp#initialize (2)
--- Interp#init (2)
--- Interp#name (0)
--- Interp#min_size (0)
--- Interp#eval (4)
--- Interp#eval_e (5)
--- Interp#eval_deriv (4)
--- Interp#eval_deriv_e (5)
--- Interp#eval_deriv2 (4)
--- Interp#eval_deriv2_e (5)
--- Interp#eval_integ (5)
--- Interp#eval_integ_e (6)
== ((:Spline:))
=== Superclass
(({Object}))
=== Class Methods
--- Spline.new (2)
=== Instance Methods
--- Spline#initialize (2)
--- Spline#init (2)
--- Spline#name (0)
--- Spline#min_size (0)
--- Spline#eval (2)
--- Spline#eval_e (3)
--- Spline#eval_deriv (2)
--- Spline#eval_deriv_e (3)
--- Spline#eval_deriv2 (2)
--- Spline#eval_deriv2_e (3)
--- Spline#eval_integ (3)
--- Spline#eval_integ_e (4)
== ((:Linalg:))
=== Module Functions
--- Linalg#QR_QRsolve (3)
--- Linalg#QR_update (4)
--- Linalg#QR_update (4)
--- Linalg#QRPT_QRsolve (4)
--- Linalg#QRPT_update (3)
--- Linalg#SV_solve (4)
--- Linalg#solve_symm_tridiag (3)
--- Linalg#solve_tridiag (4)
--- Linalg#solve_symm_cyc_tridiag (3)
--- Linalg#solve_cyc_tridiag (4)
--- Linalg#balance_columns (2)
== ((:MFunction:))
=== Superclass
(({Object}))
== ((:Matrix:))
=== Superclass
(({Object}))
=== Class Methods
--- Matrix.new (-1)
--- Matrix.alloc (-1)
--- Matrix.calloc (2)
--- Matrix.float_equal (3)
--- Matrix.perm (1)
=== Instance Methods
--- Matrix#dger (3)
--- Matrix#dsyr (3)
--- Matrix#dsyr! (3)
--- Matrix#dsyr2 (4)
--- Matrix#dsyr2! (4)
--- Matrix#dgemm (6)
--- Matrix#dgemm! (6)
--- Matrix#dsymm (6)
--- Matrix#dtrmm (6)
--- Matrix#dtrmm! (6)
--- Matrix#dtrsm (6)
--- Matrix#dtrsm! (6)
--- Matrix#dsyr2k (6)
--- Matrix#dsyr2k! (6)
--- Matrix#eigen_symm! (0)
--- Matrix#eigen_symm (0)
--- Matrix#eigen_symmv! (0)
--- Matrix#eigen_symmv (0)
--- Matrix#LU_decomp! (0)
--- Matrix#LU_decomp (0)
--- Matrix#LU_solve (2)
--- Matrix#LU_decomp_solve! (1)
--- Matrix#LUD_solve (1)
--- Matrix#LU_refine (3)
--- Matrix#LU_invert (1)
--- Matrix#invert (0)
--- Matrix#LU_det (1)
--- Matrix#det (0)
--- Matrix#LU_lndet (0)
--- Matrix#lndet (0)
--- Matrix#LU_sgndet (1)
--- Matrix#sgndet (0)
--- Matrix#QR_decomp! (0)
--- Matrix#QR_decomp (0)
--- Matrix#QR_solve (2)
--- Matrix#QR_lssolve (2)
--- Matrix#QR_QTvec (2)
--- Matrix#QR_Qvec (2)
--- Matrix#QR_Rsolve (1)
--- Matrix#QR_Rsvx (1)
--- Matrix#QR_unpack (1)
--- Matrix#R_solve (1)
--- Matrix#R_svx (1)
--- Matrix#QRPT_decomp! (0)
--- Matrix#QRPT_decomp (0)
--- Matrix#QRPT_solve (3)
--- Matrix#QRPT_Rsolve (2)
--- Matrix#SV_decomp! (0)
--- Matrix#SV_decomp (0)
--- Matrix#SV_decomp_mod! (0)
--- Matrix#SV_decomp_mod (0)
--- Matrix#SV_decomp_jacobi! (0)
--- Matrix#SV_decomp_jacobi (0)
--- Matrix#SVD_solve (1)
--- Matrix#cholesky_decomp! (0)
--- Matrix#cholesky_decomp (0)
--- Matrix#cholesky_solve (1)
--- Matrix#symmtd_decomp! (0)
--- Matrix#symmtd_decomp (0)
--- Matrix#symmtd_unpack (1)
--- Matrix#symmtd_unpack_T (0)
--- Matrix#bidiag_decomp! (0)
--- Matrix#bidiag_decomp (0)
--- Matrix#bidiag_unpack (2)
--- Matrix#bidiag_unpack2 (2)
--- Matrix#bidiag_unpack_B (0)
--- Matrix#HH_solve! (1)
--- Matrix#HH_svx! (1)
--- Matrix#HH_solve (1)
--- Matrix#HH_svx (1)
--- Matrix#householder_hm! (2)
--- Matrix#householder_mh! (2)
--- Matrix#householder_hm1 (1)
--- Matrix#U (0)
--- Matrix#L (0)
--- Matrix#upper (1)
--- Matrix#lower (1)
--- Matrix#trace (0)
--- Matrix#size1 (0)
--- Matrix#size2 (0)
--- Matrix#get (2)
--- Matrix#set (-1)
--- Matrix#set_all (1)
--- Matrix#set_zero (0)
--- Matrix#set_identity (0)
--- Matrix#submatrix (4)
--- Matrix#row (1)
--- Matrix#column (1)
--- Matrix#diagonal (0)
--- Matrix#subdiagonal (1)
--- Matrix#superdiagonal (1)
--- Matrix#clone (0)
--- Matrix#swap (1)
--- Matrix#set_row (2)
--- Matrix#set_col (2)
--- Matrix#swap_rows (2)
--- Matrix#swap_rows! (2)
--- Matrix#swap_columns (2)
--- Matrix#swap_columns! (2)
--- Matrix#swap_rowcol (2)
--- Matrix#swap_rowcol! (2)
--- Matrix#transpose_memcpy (0)
--- Matrix#transpose! (0)
--- Matrix#isnull (0)
--- Matrix#add (1)
--- Matrix#add! (1)
--- Matrix#sub (1)
--- Matrix#sub! (1)
--- Matrix#mul_elements (1)
--- Matrix#mul_elements! (1)
--- Matrix#mul (1)
--- Matrix#mul! (1)
--- Matrix#div_elements (1)
--- Matrix#div_elements! (1)
--- Matrix#scale (1)
--- Matrix#scale! (1)
--- Matrix#add_constant (1)
--- Matrix#add_constant! (1)
--- Matrix#max (0)
--- Matrix#min (0)
--- Matrix#minmax (0)
--- Matrix#max_index (0)
--- Matrix#min_index (0)
--- Matrix#minmax_index (0)
--- Matrix#== (1)
--- Matrix#fwrite (1)
--- Matrix#fread (1)
--- Matrix#fprintf (2)
--- Matrix#printf (1)
--- Matrix#fscanf (1)
--- Matrix#print (0)
--- Matrix#inspect (0)
--- Matrix#to_a (0)
--- Matrix#_load_data (1)
--- Matrix#_dump_data (0)
--- Matrix#mult (1)
--- Matrix#mult_vect (1)
--- Matrix#negative (0)
--- Matrix#to_complex (0)
== ((:Matrix_complex:))
=== Superclass
(({Object}))
=== Class Methods
--- Matrix_complex.new (-1)
--- Matrix_complex.alloc (-1)
--- Matrix_complex.calloc (2)
--- Matrix_complex.float_equal (3)
--- Matrix_complex.perm (1)
=== Instance Methods
--- Matrix_complex#zgeru (3)
--- Matrix_complex#zgeru! (3)
--- Matrix_complex#zgerc (3)
--- Matrix_complex#zgerc! (3)
--- Matrix_complex#zher (3)
--- Matrix_complex#zher! (3)
--- Matrix_complex#zher2 (4)
--- Matrix_complex#zher2! (4)
--- Matrix_complex#zgemm (6)
--- Matrix_complex#zgemm! (6)
--- Matrix_complex#zsymm (6)
--- Matrix_complex#zsymm! (6)
--- Matrix_complex#zhemm (6)
--- Matrix_complex#zhemm! (6)
--- Matrix_complex#ztrmm (6)
--- Matrix_complex#ztrmm! (6)
--- Matrix_complex#ztrsm (6)
--- Matrix_complex#ztrsm! (6)
--- Matrix_complex#zsyr2k (6)
--- Matrix_complex#zsyr2k! (6)
--- Matrix_complex#zher2k (6)
--- Matrix_complex#zher2k! (6)
--- Matrix_complex#eigen_herm! (0)
--- Matrix_complex#eigen_herm (0)
--- Matrix_complex#eigen_hermv! (0)
--- Matrix_complex#eigen_hermv (0)
--- Matrix_complex#LU_decomp! (0)
--- Matrix_complex#LU_decomp (0)
--- Matrix_complex#LU_solve (2)
--- Matrix_complex#LU_decomp_solve! (1)
--- Matrix_complex#LUD_solve (1)
--- Matrix_complex#LU_refine (3)
--- Matrix_complex#LU_invert (1)
--- Matrix_complex#invert (0)
--- Matrix_complex#LU_det (1)
--- Matrix_complex#det (0)
--- Matrix_complex#LU_lndet (0)
--- Matrix_complex#lndet (0)
--- Matrix_complex#LU_sgndet (1)
--- Matrix_complex#hermtd_decomp! (0)
--- Matrix_complex#hermtd_decomp (0)
--- Matrix_complex#hermtd_unpack (1)
--- Matrix_complex#hermtd_unpack_T (0)
--- Matrix_complex#householder_hm! (2)
--- Matrix_complex#householder_mh! (2)
--- Matrix_complex#householder_hm1 (1)
--- Matrix_complex#U (0)
--- Matrix_complex#L (0)
--- Matrix_complex#upper (1)
--- Matrix_complex#lower (1)
--- Matrix_complex#trace (0)
--- Matrix_complex#size1 (0)
--- Matrix_complex#size2 (0)
--- Matrix_complex#get (2)
--- Matrix_complex#set1 (3)
--- Matrix_complex#set (-1)
--- Matrix_complex#set_all (1)
--- Matrix_complex#set_zero (0)
--- Matrix_complex#set_identity (0)
--- Matrix_complex#submatrix (4)
--- Matrix_complex#row (1)
--- Matrix_complex#column (1)
--- Matrix_complex#diagonal (0)
--- Matrix_complex#subdiagonal (1)
--- Matrix_complex#superdiagonal (1)
--- Matrix_complex#clone (0)
--- Matrix_complex#swap (1)
--- Matrix_complex#set_row (2)
--- Matrix_complex#set_col (2)
--- Matrix_complex#swap_rows (2)
--- Matrix_complex#swap_rows! (2)
--- Matrix_complex#swap_columns (2)
--- Matrix_complex#swap_columns! (2)
--- Matrix_complex#swap_rowcol (2)
--- Matrix_complex#swap_rowcol! (2)
--- Matrix_complex#transpose_memcpy (0)
--- Matrix_complex#transpose! (0)
--- Matrix_complex#herm (0)
--- Matrix_complex#isnull (0)
--- Matrix_complex#add (1)
--- Matrix_complex#add! (1)
--- Matrix_complex#sub (1)
--- Matrix_complex#sub! (1)
--- Matrix_complex#mul_elements (1)
--- Matrix_complex#mul_elements! (1)
--- Matrix_complex#mul (1)
--- Matrix_complex#mul! (1)
--- Matrix_complex#div_elements (1)
--- Matrix_complex#div_elements! (1)
--- Matrix_complex#scale (1)
--- Matrix_complex#scale! (1)
--- Matrix_complex#== (1)
--- Matrix_complex#fwrite (1)
--- Matrix_complex#fread (1)
--- Matrix_complex#fprintf (2)
--- Matrix_complex#printf (1)
--- Matrix_complex#fscanf (1)
--- Matrix_complex#print (0)
--- Matrix_complex#inspect (0)
--- Matrix_complex#to_a (0)
--- Matrix_complex#_load_data (1)
--- Matrix_complex#_dump_data (0)
--- Matrix_complex#mult (1)
--- Matrix_complex#mult_vect (1)
--- Matrix_complex#negative (0)
--- Matrix_complex#upper (1)
--- Matrix_complex#lower (1)
== ((:Minimizer:))
=== Superclass
(({Object}))
=== Class Methods
--- Minimizer.new (1)
=== Instance Methods
--- Minimizer#initialize (1)
--- Minimizer#set (4)
--- Minimizer#set_with_values (7)
--- Minimizer#name (0)
--- Minimizer#iterate (0)
--- Minimizer#x_minimum (0)
--- Minimizer#x_lower (0)
--- Minimizer#x_upper (0)
--- Minimizer#f_minimum (0)
--- Minimizer#f_lower (0)
--- Minimizer#f_upper (0)
=== Module Functions
--- Minimizer#test_interval (4)
=== Constants
--- BRENT
--- GOLDENSECTION
== ((:MonteCarlo:))
== ((:Function:))
=== Superclass
(({Object}))
=== Class Methods
--- Function.new (3)
--- Function.new (0)
--- Function.new2 (1)
--- Function.alloc (0)
=== Instance Methods
--- Function#initialize (3)
--- Function#eval (-1)
--- Function#eval2 (1)
--- Function#eval3 (-1)
--- Function#eval (2)
--- Function#arity (0)
--- Function#proc (0)
--- Function#set (0)
== ((:Miser:))
=== Superclass
(({Object}))
=== Class Methods
--- Miser.new (1)
=== Instance Methods
--- Miser#initialize (1)
--- Miser#integrate (5)
--- Miser#estimate_frac (0)
--- Miser#estimate_frac= (1)
--- Miser#min_calls (0)
--- Miser#min_calls= (1)
--- Miser#min_calls_per_bisection (0)
--- Miser#min_calls_per_bisection= (1)
--- Miser#alpha (0)
--- Miser#alpha= (1)
--- Miser#dither (0)
--- Miser#dither= (1)
== ((:Plain:))
=== Superclass
(({Object}))
=== Class Methods
--- Plain.new (1)
=== Instance Methods
--- Plain#initialize (1)
--- Plain#integrate (5)
== ((:Vegas:))
=== Superclass
(({Object}))
=== Class Methods
--- Vegas.new (1)
=== Instance Methods
--- Vegas#initialize (1)
--- Vegas#integrate (5)
--- Vegas#result (0)
--- Vegas#sigma (0)
--- Vegas#iterations (0)
--- Vegas#iterations= (1)
--- Vegas#alpha (0)
--- Vegas#alpha= (1)
--- Vegas#chisq (0)
--- Vegas#chisq= (1)
--- Vegas#stage (0)
--- Vegas#stage= (1)
--- Vegas#mode (0)
--- Vegas#mode= (1)
=== Constants
--- MODE_IMPORTANCE
--- MODE_IMPORTANCE_ONLY
--- MODE_STRATIFIED
== ((:Multiroot:))
=== Module Functions
--- Multiroot#test_residual (2)
--- Multiroot#test_delta (4)
== ((:MFDFSolver:))
=== Superclass
(({Object}))
=== Class Methods
--- MFDFSolver.new (2)
=== Instance Methods
--- MFDFSolver#initialize (2)
--- MFDFSolver#set (2)
--- MFDFSolver#name (0)
--- MFDFSolver#iterate (0)
--- MFDFSolver#root (0)
--- MFDFSolver#dx (0)
--- MFDFSolver#f (0)
=== Constants
--- GNEWTON
--- HYBRIDJ
--- HYBRIDSJ
--- NEWTON
== ((:MFSolver:))
=== Superclass
(({Object}))
=== Class Methods
--- MFSolver.new (2)
=== Instance Methods
--- MFSolver#initialize (2)
--- MFSolver#set (2)
--- MFSolver#name (0)
--- MFSolver#iterate (0)
--- MFSolver#root (0)
--- MFSolver#dx (0)
--- MFSolver#f (0)
=== Constants
--- BISECTION
--- BRENT
--- BROYDEN
--- DNEWTON
--- FALSEPOS
--- HYBRID
--- HYBRIDS
== ((:Permutation:))
=== Superclass
(({Object}))
=== Class Methods
--- Permutation.new (1)
--- Permutation.new2 (1)
--- Permutation.new_ary (1)
=== Instance Methods
--- Permutation#initialize (1)
--- Permutation#reset (0)
--- Permutation#size (0)
--- Permutation#valid (0)
--- Permutation#cycles (0)
--- Permutation#equal (1)
--- Permutation#get (1)
--- Permutation#data (0)
--- Permutation#swap (2)
--- Permutation#reverse! (0)
--- Permutation#inverse (0)
--- Permutation#next (0)
--- Permutation#prev (0)
--- Permutation#each! (0)
--- Permutation#each (0)
--- Permutation#mul (1)
--- Permutation#fwrite (1)
--- Permutation#fread (1)
--- Permutation#fprintf (2)
--- Permutation#fscanf (1)
--- Permutation#dump (0)
--- Permutation#undump (0)
--- Permutation#to_canonical (0)
--- Permutation#to_linear (0)
=== Module Functions
--- Permutation#memcpy (2)
== ((:Poly:))
=== Module Functions
--- Poly#evaluate (2)
--- Poly#solve_quadratic (3)
--- Poly#complex_solve_quadratic (3)
--- Poly#solve_cubic (3)
--- Poly#complex_solve_cubic (3)
== ((:DD:))
=== Superclass
(({Object}))
=== Class Methods
--- DD.new (2)
=== Instance Methods
--- DD#initialize (2)
--- DD#evaluate (1)
--- DD#get (0)
--- DD#taylor (1)
== ((:Workspace:))
=== Superclass
(({Object}))
=== Class Methods
--- Workspace.new (1)
=== Instance Methods
--- Workspace#initialize (1)
--- Workspace#solve (1)
== ((:Random:))
== ((:Discrete:))
=== Superclass
(({Object}))
=== Class Methods
--- Discrete.new (1)
=== Instance Methods
--- Discrete#initialize (1)
--- Discrete#get (1)
--- Discrete#get_pdf (1)
== ((:QRNG:))
=== Superclass
(({Object}))
=== Class Methods
--- QRNG.new (2)
=== Instance Methods
--- QRNG#initialize (2)
--- QRNG#reset (0)
--- QRNG#get (0)
--- QRNG#name (0)
--- QRNG#dim (0)
--- QRNG#size (0)
--- QRNG#clone (0)
=== Module Functions
--- QRNG#memcpy (2)
=== Constants
--- NIEDERREITER_2
--- SOBOL
== ((:RND:))
=== Module Functions
--- RND#gaussian (2)
--- RND#gaussian_ratio_method (2)
--- RND#gaussian_pdf (2)
--- RND#ugaussian (1)
--- RND#ugaussian_ratio_method (1)
--- RND#ugaussian_pdf (1)
--- RND#gaussian_tail (3)
--- RND#gaussian_tail_pdf (3)
--- RND#ugaussian_tail (2)
--- RND#ugaussian_tail_pdf (2)
--- RND#bivariate_gaussian (4)
--- RND#bivariate_gaussian_pdf (5)
--- RND#exponential (2)
--- RND#exponential_pdf (2)
--- RND#laplace (2)
--- RND#laplace_pdf (2)
--- RND#exppow (3)
--- RND#exppow_pdf (3)
--- RND#cauchy (2)
--- RND#cauchy_pdf (2)
--- RND#rayleigh (2)
--- RND#rayleigh_pdf (2)
--- RND#rayleigh_tail (3)
--- RND#rayleigh_tail_pdf (3)
--- RND#landau (1)
--- RND#landau_pdf (1)
--- RND#levy (3)
--- RND#levy_skew (4)
--- RND#gamma (3)
--- RND#gamma_int (2)
--- RND#gamma_pdf (3)
--- RND#flat (3)
--- RND#flat_pdf (3)
--- RND#lognormal (3)
--- RND#lognormal_pdf (3)
--- RND#chisq (2)
--- RND#chisq_pdf (2)
--- RND#fdist (3)
--- RND#fdist_pdf (3)
--- RND#tdist (2)
--- RND#tdist_pdf (2)
--- RND#beta (3)
--- RND#beta_pdf (3)
--- RND#logistic (2)
--- RND#logistic_pdf (2)
--- RND#pareto (3)
--- RND#pareto_pdf (3)
--- RND#dir_2d (1)
--- RND#dir_2d_trig_method (1)
--- RND#dir_3d (1)
--- RND#dir_nd (2)
--- RND#weibull (3)
--- RND#weibull_pdf (3)
--- RND#gumbel1 (3)
--- RND#gumbel1_pdf (3)
--- RND#gumbel2 (3)
--- RND#gumbel2_pdf (3)
--- RND#erlang (3)
--- RND#erlang_pdf (3)
--- RND#poisson (2)
--- RND#poisson_pdf (2)
--- RND#poisson_array (3)
--- RND#bernoulli (2)
--- RND#bernoulli_pdf (2)
--- RND#binomial (3)
--- RND#binomial_pdf (3)
--- RND#negative_binomial (3)
--- RND#negative_binomial_pdf (3)
--- RND#pascal (3)
--- RND#pascal_pdf (3)
--- RND#geometric (2)
--- RND#geometric_pdf (2)
--- RND#hypergeometric (4)
--- RND#hypergeometric_pdf (4)
--- RND#logarithmic (2)
--- RND#logarithmic_pdf (2)
--- RND#dirichlet (4)
--- RND#dirichlet_pdf (3)
--- RND#dirichlet_lnpdf (3)
--- RND#multinomial (5)
--- RND#multinomial_pdf (3)
--- RND#multinomial_lnpdf (3)
--- RND#shuffle (2)
--- RND#choose (3)
--- RND#sample (3)
== ((:RNG:))
=== Superclass
(({Object}))
=== Class Methods
--- RNG.new (0)
--- RNG.new2 (1)
=== Instance Methods
--- RNG#initialize (1)
--- RNG#set (1)
--- RNG#get (0)
--- RNG#uniform (0)
--- RNG#uniform_pos (0)
--- RNG#uniform_int (1)
--- RNG#max (0)
--- RNG#min (0)
--- RNG#name (0)
--- RNG#size (0)
--- RNG#clone (0)
--- RNG#print_state (0)
=== Module Functions
--- RNG#memcpy (2)
=== Constants
--- BOROSH13
--- CMRG
--- COVEYOU
--- FISHMAN18
--- FISHMAN20
--- FISHMAN2X
--- GFSR4
--- KNUTHRAN
--- KNUTHRAN2
--- LECUYER21
--- MINSTD
--- MRG
--- MT19937
--- MT19937_1998
--- MT19937_1999
--- R250
--- RAN0
--- RAN1
--- RAN2
--- RAN3
--- RAND
--- RAND48
--- RANDOM128_BSD
--- RANDOM128_GLIBC2
--- RANDOM128_LIBC5
--- RANDOM256_BSD
--- RANDOM256_GLIBC2
--- RANDOM256_LIBC5
--- RANDOM32_BSD
--- RANDOM32_GLIBC2
--- RANDOM32_LIBC5
--- RANDOM64_BSD
--- RANDOM64_GLIBC2
--- RANDOM64_LIBC5
--- RANDOM8_BSD
--- RANDOM8_GLIBC2
--- RANDOM8_LIBC5
--- RANDOM_BSD
--- RANDOM_GLIBC2
--- RANDOM_LIBC5
--- RANDU
--- RANF
--- RANLUX
--- RANLUX389
--- RANLXD1
--- RANLXD2
--- RANLXS0
--- RANLXS1
--- RANLXS2
--- RANMAR
--- SLATEC
--- TAUS
--- TAUS113
--- TAUS2
--- TRANSPUTER
--- TT800
--- UNI
--- UNI32
--- VAX
--- WATERMAN14
--- ZUF
== ((:Result:))
=== Superclass
(({Object}))
=== Class Methods
--- Result.new (0)
--- Result.new2 (2)
=== Instance Methods
--- Result#initialize (2)
--- Result#val (0)
--- Result#err (0)
--- Result#* (1)
== ((:Result10:))
=== Superclass
(({Object}))
=== Class Methods
--- Result10.new (0)
=== Instance Methods
--- Result10#initialize (0)
--- Result10#val (0)
--- Result10#err (0)
--- Result10#e10 (0)
== ((:Siman:))
=== Superclass
(({Object}))
=== Class Methods
--- Siman.new (-1)
=== Instance Methods
--- Siman#initialize (-1)
--- Siman#search (2)
--- Siman#set_energy (1)
--- Siman#set_metric (1)
--- Siman#set_step (1)
--- Siman#set_print (1)
== ((:Solver:))
=== Module Functions
--- Solver#test_interval (4)
--- Solver#test_residual (2)
--- Solver#test_delta (4)
== ((:FDFSolver:))
=== Superclass
(({Object}))
=== Class Methods
--- FDFSolver.new (1)
=== Instance Methods
--- FDFSolver#initialize (1)
--- FDFSolver#set (3)
--- FDFSolver#name (0)
--- FDFSolver#iterate (0)
--- FDFSolver#root (0)
=== Constants
--- NEWTON
--- SECANT
--- STEFFENSON
== ((:FSolver:))
=== Superclass
(({Object}))
=== Class Methods
--- FSolver.new (1)
=== Instance Methods
--- FSolver#initialize (1)
--- FSolver#set (3)
--- FSolver#name (0)
--- FSolver#iterate (0)
--- FSolver#root (0)
--- FSolver#x_lower (0)
--- FSolver#x_upper (0)
=== Constants
--- BISECTION
--- BRENT
--- FALSEPOS
== ((:Special:))
=== Constants
--- PREC_APPROX
--- PREC_DOUBLE
--- PREC_SINGLE
== ((:Airy:))
=== Module Functions
--- Airy#Ai (2)
--- Airy#Ai_e (2)
--- Airy#Bi (2)
--- Airy#Bi_e (2)
--- Airy#Ai_scaled (2)
--- Airy#Ai_scaled_e (2)
--- Airy#Bi_scaled (2)
--- Airy#Bi_scaled_e (2)
--- Airy#Ai_deriv (2)
--- Airy#Ai_deriv_e (2)
--- Airy#Bi_deriv (2)
--- Airy#Bi_deriv_e (2)
--- Airy#Ai_deriv_scaled (2)
--- Airy#Ai_deriv_scaled_e (2)
--- Airy#Bi_deriv_scaled (2)
--- Airy#Bi_deriv_scaled_e (2)
--- Airy#zero_Ai (1)
--- Airy#zero_Ai_e (1)
--- Airy#zero_Bi (1)
--- Airy#zero_Bi_e (1)
--- Airy#zero_Ai_deriv (1)
--- Airy#zero_Ai_deriv_e (1)
--- Airy#zero_Bi_deriv (1)
--- Airy#zero_Bi_deriv_e (1)
== ((:Bessel:))
=== Module Functions
--- Bessel#J0 (1)
--- Bessel#J0_e (1)
--- Bessel#J1 (1)
--- Bessel#J1_e (1)
--- Bessel#Jn (2)
--- Bessel#Jn_e (2)
--- Bessel#Jn_array (3)
--- Bessel#Y0 (1)
--- Bessel#Y0_e (1)
--- Bessel#Y1 (1)
--- Bessel#Y1_e (1)
--- Bessel#Yn (2)
--- Bessel#Yn_e (2)
--- Bessel#Yn_array (3)
--- Bessel#I0 (1)
--- Bessel#I0_e (1)
--- Bessel#I1 (1)
--- Bessel#I1_e (1)
--- Bessel#In (2)
--- Bessel#In_e (2)
--- Bessel#In_array (3)
--- Bessel#I0_scaled (1)
--- Bessel#I0_scaled_e (1)
--- Bessel#I1_scaled (1)
--- Bessel#I1_scaled_e (1)
--- Bessel#In_scaled (2)
--- Bessel#In_scaled_e (2)
--- Bessel#In_scaled_array (3)
--- Bessel#K0 (1)
--- Bessel#K0_e (1)
--- Bessel#K1 (1)
--- Bessel#K1_e (1)
--- Bessel#Kn (2)
--- Bessel#Kn_e (2)
--- Bessel#Kn_array (3)
--- Bessel#K0_scaled (1)
--- Bessel#K0_scaled_e (1)
--- Bessel#K1_scaled (1)
--- Bessel#K1_scaled_e (1)
--- Bessel#Kn_scaled (2)
--- Bessel#Kn_scaled_e (2)
--- Bessel#Kn_scaled_array (3)
--- Bessel#j0 (1)
--- Bessel#j0_e (1)
--- Bessel#j1 (1)
--- Bessel#j1_e (1)
--- Bessel#j2 (1)
--- Bessel#j2_e (1)
--- Bessel#jl (2)
--- Bessel#jl_e (2)
--- Bessel#jl_array (2)
--- Bessel#jl_steed_array (2)
--- Bessel#y0 (1)
--- Bessel#y0_e (1)
--- Bessel#y1 (1)
--- Bessel#y1_e (1)
--- Bessel#y2 (1)
--- Bessel#y2_e (1)
--- Bessel#yl (2)
--- Bessel#yl_e (2)
--- Bessel#yl_array (2)
--- Bessel#i0_scaled (1)
--- Bessel#i0_scaled_e (1)
--- Bessel#i1_scaled (1)
--- Bessel#i1_scaled_e (1)
--- Bessel#i2_scaled (1)
--- Bessel#i2_scaled_e (1)
--- Bessel#il_scaled (2)
--- Bessel#il_scaled_e (2)
--- Bessel#il_scaled_array (2)
--- Bessel#k0_scaled (1)
--- Bessel#k0_scaled_e (1)
--- Bessel#k1_scaled (1)
--- Bessel#k1_scaled_e (1)
--- Bessel#k2_scaled (1)
--- Bessel#k2_scaled_e (1)
--- Bessel#kl_scaled (2)
--- Bessel#kl_scaled_e (2)
--- Bessel#kl_scaled_array (2)
--- Bessel#Jnu (2)
--- Bessel#Jnu_e (2)
--- Bessel#Ynu (2)
--- Bessel#Ynu_e (2)
--- Bessel#Inu (2)
--- Bessel#Inu_e (2)
--- Bessel#Inu_scaled (2)
--- Bessel#Inu_scaled_e (2)
--- Bessel#Knu (2)
--- Bessel#Knu_e (2)
--- Bessel#Knu_scaled (2)
--- Bessel#Knu_scaled_e (2)
--- Bessel#lnKnu (2)
--- Bessel#lnKnu_e (2)
--- Bessel#zero_J0 (1)
--- Bessel#zero_J0_e (1)
--- Bessel#zero_J1 (1)
--- Bessel#zero_J1_e (1)
--- Bessel#zero_Jnu (2)
--- Bessel#zero_Jnu_e (2)
== ((:Clausen:))
=== Module Functions
--- Clausen#clausen (1)
--- Clausen#clausen_e (1)
== ((:Conical:))
=== Module Functions
--- Conical#P_half (2)
--- Conical#P_half_e (2)
--- Conical#P_mhalf (2)
--- Conical#P_mhalf_e (2)
--- Conical#P_0 (2)
--- Conical#P_0_e (2)
--- Conical#P_1 (2)
--- Conical#P_1_e (2)
--- Conical#P_sph_reg (3)
--- Conical#P_sph_reg_e (3)
--- Conical#P_cyl_reg (3)
--- Conical#P_cyl_reg_e (3)
== ((:Coulomb:))
=== Module Functions
--- Coulomb#hydrogenicR (4)
--- Coulomb#hydrogenicR_e (4)
--- Coulomb#hydrogenicR_1 (2)
--- Coulomb#hydrogenicR_1_e (2)
--- Coulomb#CL_e (2)
--- Coulomb#CL_array (3)
== ((:Coupling:))
=== Module Functions
--- Coupling#wigner_3j (6)
--- Coupling#wigner_3j_e (6)
--- Coupling#wigner_6j (6)
--- Coupling#wigner_6j_e (6)
--- Coupling#wigner_9j (9)
--- Coupling#wigner_9j_e (9)
--- Coupling#racahW (6)
--- Coupling#racahW_e (6)
== ((:Dawson:))
=== Module Functions
--- Dawson#dawson (1)
--- Dawson#dawson_e (1)
== ((:Debye:))
=== Module Functions
--- Debye#D1 (1)
--- Debye#D1_e (1)
--- Debye#D2 (1)
--- Debye#D2_e (1)
--- Debye#D3 (1)
--- Debye#D3_e (1)
--- Debye#D4 (1)
--- Debye#D4_e (1)
== ((:Dilog:))
=== Module Functions
--- Dilog#dilog (1)
--- Dilog#dilog_e (1)
--- Dilog#complex_dilog_e (2)
== ((:Elementary:))
=== Module Functions
--- Elementary#multiply (2)
--- Elementary#multiply_e (2)
--- Elementary#multiply_err_e (4)
== ((:EllInt:))
=== Module Functions
--- EllInt#Kcomp (2)
--- EllInt#Kcomp_e (2)
--- EllInt#Ecomp (2)
--- EllInt#Ecomp_e (2)
--- EllInt#E (3)
--- EllInt#E_e (3)
--- EllInt#F (3)
--- EllInt#F_e (3)
--- EllInt#P (4)
--- EllInt#P_e (4)
--- EllInt#D (4)
--- EllInt#D_e (4)
--- EllInt#RC (3)
--- EllInt#RC_e (3)
--- EllInt#RD (4)
--- EllInt#RD_e (4)
--- EllInt#RF (4)
--- EllInt#RF_e (4)
--- EllInt#RJ (5)
--- EllInt#RJ_e (5)
== ((:EllJac:))
=== Module Functions
--- EllJac#e (2)
== ((:Error:))
=== Module Functions
--- Error#erf (1)
--- Error#erf_e (1)
--- Error#erfc (1)
--- Error#erfc_e (1)
--- Error#log_erf (1)
--- Error#log_erf_e (1)
--- Error#erf_Z (1)
--- Error#erf_Z_e (1)
--- Error#erf_Q (1)
--- Error#erf_Q_e (1)
== ((:Exp:))
=== Module Functions
--- Exp#exp (1)
--- Exp#exp_e (1)
--- Exp#exp_e10_e (1)
--- Exp#exp_mult (2)
--- Exp#exp_mult_e (2)
--- Exp#exp_mult_e10_e (2)
--- Exp#expm1 (1)
--- Exp#expm1_e (1)
--- Exp#exprel (1)
--- Exp#exprel_e (1)
--- Exp#exprel_2 (1)
--- Exp#exprel_2_e (1)
--- Exp#exprel_n (2)
--- Exp#exprel_n_e (2)
--- Exp#exp_err_e (2)
--- Exp#exp_err_e10_e (2)
--- Exp#exp_mult_err_e (4)
--- Exp#exp_mult_err_e10_e (4)
== ((:ExpInt:))
=== Module Functions
--- ExpInt#E1 (1)
--- ExpInt#E1_e (1)
--- ExpInt#E2 (1)
--- ExpInt#E2_e (1)
--- ExpInt#E1_scaled (1)
--- ExpInt#E1_scaled_e (1)
--- ExpInt#E2_scaled (1)
--- ExpInt#E2_scaled_e (1)
--- ExpInt#Ei (1)
--- ExpInt#Ei_e (1)
--- ExpInt#Ei_scaled (1)
--- ExpInt#Ei_scaled_e (1)
--- ExpInt#Shi (1)
--- ExpInt#Shi_e (1)
--- ExpInt#Chi (1)
--- ExpInt#Chi_e (1)
--- ExpInt#Ei3 (1)
--- ExpInt#Ei3_e (1)
--- ExpInt#Si (1)
--- ExpInt#Si_e (1)
--- ExpInt#Ci (1)
--- ExpInt#Ci_e (1)
--- ExpInt#atanint (1)
--- ExpInt#atanint_e (1)
== ((:FermiDirac:))
=== Module Functions
--- FermiDirac#Fm1 (1)
--- FermiDirac#Fm1_e (1)
--- FermiDirac#F0 (1)
--- FermiDirac#F0_e (1)
--- FermiDirac#F1 (1)
--- FermiDirac#F1_e (1)
--- FermiDirac#F2 (1)
--- FermiDirac#F2_e (1)
--- FermiDirac#Fmhalf (1)
--- FermiDirac#Fmhalf_e (1)
--- FermiDirac#Fhalf (1)
--- FermiDirac#Fhalf_e (1)
--- FermiDirac#F3half (1)
--- FermiDirac#F3half_e (1)
--- FermiDirac#Fint (2)
--- FermiDirac#Fint_e (2)
--- FermiDirac#Finc_0 (2)
--- FermiDirac#Finc_0_e (2)
== ((:Gamma:))
=== Module Functions
--- Gamma#gamma (1)
--- Gamma#gamma_e (1)
--- Gamma#lngamma (1)
--- Gamma#lngamma_e (1)
--- Gamma#gammastar (1)
--- Gamma#gammastar_e (1)
--- Gamma#gammainv (1)
--- Gamma#gammainv_e (1)
--- Gamma#taylorcoeff (2)
--- Gamma#taylorcoeff_e (2)
--- Gamma#fact (1)
--- Gamma#fact_e (1)
--- Gamma#doublefact (1)
--- Gamma#doublefact_e (1)
--- Gamma#lnfact (1)
--- Gamma#lnfact_e (1)
--- Gamma#lndoublefact (1)
--- Gamma#lndoublefact_e (1)
--- Gamma#lnchoose (2)
--- Gamma#lnchoose_e (2)
--- Gamma#choose (2)
--- Gamma#choose_e (2)
--- Gamma#lnpoch (2)
--- Gamma#lnpoch_e (2)
--- Gamma#lnpoch_sgn_e (2)
--- Gamma#poch (2)
--- Gamma#poch_e (2)
--- Gamma#pochrel (2)
--- Gamma#pochrel_e (2)
--- Gamma#gamma_inc_Q (2)
--- Gamma#gamma_inc_Q_e (2)
--- Gamma#gamma_inc_P (2)
--- Gamma#gamma_inc_P_e (2)
--- Gamma#lnbeta (2)
--- Gamma#lnbeta_e (2)
--- Gamma#beta (2)
--- Gamma#beta_e (2)
--- Gamma#beta_inc (3)
--- Gamma#beta_inc_e (3)
== ((:Gegenbauer:))
=== Module Functions
--- Gegenbauer#poly_1 (2)
--- Gegenbauer#poly_1_e (2)
--- Gegenbauer#poly_2 (2)
--- Gegenbauer#poly_2_e (2)
--- Gegenbauer#poly_3 (2)
--- Gegenbauer#poly_3_e (2)
--- Gegenbauer#poly_n (3)
--- Gegenbauer#poly_n_e (3)
--- Gegenbauer#poly_array (3)
== ((:Hyperg:))
=== Module Functions
--- Hyperg#F01 (2)
--- Hyperg#F01_e (2)
--- Hyperg#F11_int (3)
--- Hyperg#F11_int_e (3)
--- Hyperg#F11 (3)
--- Hyperg#F11_e (3)
--- Hyperg#U_int (3)
--- Hyperg#U_int_e (3)
--- Hyperg#U (3)
--- Hyperg#U_e (3)
--- Hyperg#F21 (4)
--- Hyperg#F21_e (4)
--- Hyperg#F21_conj (4)
--- Hyperg#F21_conj_e (4)
--- Hyperg#F21_renorm (4)
--- Hyperg#F21_renorm_e (4)
--- Hyperg#F21_conj_renorm (4)
--- Hyperg#F21_conj_renorm_e (4)
--- Hyperg#F20 (3)
--- Hyperg#F20_e (3)
== ((:Laguerre:))
=== Module Functions
--- Laguerre#L1 (2)
--- Laguerre#L1_e (2)
--- Laguerre#L2 (2)
--- Laguerre#L2_e (2)
--- Laguerre#L3 (2)
--- Laguerre#L3_e (2)
--- Laguerre#Ln (3)
--- Laguerre#Ln_e (3)
== ((:Lambert:))
=== Module Functions
--- Lambert#W0 (1)
--- Lambert#W0_e (1)
--- Lambert#Wm1 (1)
--- Lambert#Wm1_e (1)
== ((:Legendre:))
=== Module Functions
--- Legendre#P1 (1)
--- Legendre#P1_e (1)
--- Legendre#P2 (1)
--- Legendre#P2_e (1)
--- Legendre#P3 (1)
--- Legendre#P3_e (1)
--- Legendre#Pl (2)
--- Legendre#Pl_e (2)
--- Legendre#Pl_array (2)
--- Legendre#Q0 (1)
--- Legendre#Q0_e (1)
--- Legendre#Q1 (1)
--- Legendre#Q1_e (1)
--- Legendre#Ql (2)
--- Legendre#Ql_e (2)
--- Legendre#Plm (3)
--- Legendre#Plm_e (3)
--- Legendre#Plm_array (3)
--- Legendre#sphPlm (3)
--- Legendre#sphPlm_e (3)
--- Legendre#sphPlm_array (3)
--- Legendre#H3d_0 (2)
--- Legendre#H3d_0_e (2)
--- Legendre#H3d_1 (2)
--- Legendre#H3d_1_e (2)
--- Legendre#H3d (3)
--- Legendre#H3d_e (3)
== ((:Log:))
=== Module Functions
--- Log#log (1)
--- Log#log_e (1)
--- Log#log_abs (1)
--- Log#log_abs_e (1)
--- Log#log_1px (1)
--- Log#log_1px_e (1)
--- Log#log_1px_mx (1)
--- Log#log_1px_mx_e (1)
== ((:Psi:))
=== Module Functions
--- Psi#psi (1)
--- Psi#psi_e (1)
--- Psi#psi_int (1)
--- Psi#psi_int_e (1)
--- Psi#psi_1piy (1)
--- Psi#psi_1piy_e (1)
--- Psi#psi_1_int (1)
--- Psi#psi_1_int_e (1)
--- Psi#psi_n (2)
--- Psi#psi_n_e (2)
== ((:Synchrotron:))
=== Module Functions
--- Synchrotron#S1 (1)
--- Synchrotron#S1_e (1)
--- Synchrotron#S2 (1)
--- Synchrotron#S2_e (1)
== ((:Transport:))
=== Module Functions
--- Transport#J2 (1)
--- Transport#J2_e (1)
--- Transport#J3 (1)
--- Transport#J3_e (1)
--- Transport#J4 (1)
--- Transport#J4_e (1)
--- Transport#J5 (1)
--- Transport#J5_e (1)
== ((:Trig:))
=== Module Functions
--- Trig#sin (1)
--- Trig#sin_e (1)
--- Trig#cos (1)
--- Trig#cos_e (1)
--- Trig#hypot (2)
--- Trig#hypot_e (2)
--- Trig#sinc (1)
--- Trig#sinc_e (1)
--- Trig#complex_sin_e (2)
--- Trig#complex_cos_e (2)
--- Trig#complex_logsin_e (2)
--- Trig#lnsinh (1)
--- Trig#lnsinh_e (1)
--- Trig#lncosh (1)
--- Trig#lncosh_e (1)
--- Trig#polar2rect (2)
--- Trig#rect2polar (2)
--- Trig#angle_restrict_symm (1)
--- Trig#angle_restrict_symm_err_e (1)
--- Trig#angle_restrict_pos (1)
--- Trig#angle_restrict_pos_err_e (1)
--- Trig#sin_err_e (2)
--- Trig#cos_err_e (2)
== ((:Zeta:))
=== Module Functions
--- Zeta#zeta (1)
--- Zeta#zeta_e (1)
--- Zeta#zeta_int (1)
--- Zeta#zeta_int_e (1)
--- Zeta#hzeta (2)
--- Zeta#hzeta_e (2)
--- Zeta#eta (1)
--- Zeta#eta_e (1)
--- Zeta#eta_int (1)
--- Zeta#eta_int_e (1)
== ((:Stats:))
=== Module Functions
--- Stats#mean (2)
--- Stats#mean1 (1)
--- Stats#variance (2)
--- Stats#variance1 (1)
--- Stats#variance1_m (2)
--- Stats#variance_m (3)
--- Stats#variance_with_fixed_mean (3)
--- Stats#variance1_with_fixed_mean (2)
--- Stats#sd (2)
--- Stats#sd1 (1)
--- Stats#sd_m (3)
--- Stats#sd1_m (2)
--- Stats#sd_with_fixed_mean (3)
--- Stats#sd1_with_fixed_mean (2)
--- Stats#absdev (2)
--- Stats#absdev_m (3)
--- Stats#skew (2)
--- Stats#skew_m_sd (4)
--- Stats#kurtosis (2)
--- Stats#kurtosis_m_sd (4)
--- Stats#lag1_autocorrelation (2)
--- Stats#lag1_autocorrelation_m (3)
--- Stats#covariance (4)
--- Stats#covariance_m (6)
--- Stats#wmean (4)
--- Stats#wvariance (4)
--- Stats#wvariance_with_fixed_mean (5)
--- Stats#wvariance_m (5)
--- Stats#wsd (4)
--- Stats#wsd_m (5)
--- Stats#wsd_with_fixed_mean (5)
--- Stats#wabsdev (4)
--- Stats#wabsdev_m (5)
--- Stats#wskew (4)
--- Stats#wskew_m_sd (6)
--- Stats#wkurtosis (4)
--- Stats#wkurtosis_m_sd (6)
--- Stats#max (2)
--- Stats#min (2)
--- Stats#minmax (2)
--- Stats#max_index (2)
--- Stats#min_index (2)
--- Stats#minmax_index (2)
--- Stats#median (2)
--- Stats#median_from_sorted_data (2)
--- Stats#quantile (3)
--- Stats#quantile_from_sorted_data (3)
--- Stats#pvariance (4)
--- Stats#ttest (4)
== ((:Sum:))
== ((:Levin:))
=== Superclass
(({Object}))
=== Class Methods
--- Levin.new (1)
=== Instance Methods
--- Levin#initialize (1)
--- Levin#size (0)
--- Levin#sum_plain (0)
--- Levin#terms_used (0)
--- Levin#accel (1)
--- Levin#minmax (3)
--- Levin#step (3)
== ((:LevinTrunc:))
=== Superclass
(({Object}))
=== Class Methods
--- LevinTrunc.new (1)
=== Instance Methods
--- LevinTrunc#initialize (1)
--- LevinTrunc#size (0)
--- LevinTrunc#sum_plain (0)
--- LevinTrunc#terms_used (0)
--- LevinTrunc#accel (1)
--- LevinTrunc#minmax (3)
--- LevinTrunc#step (2)
== ((:Vector:))
=== Superclass
(({Object}))
=== Class Methods
--- Vector.new (1)
--- Vector.alloc (1)
--- Vector.calloc (1)
--- Vector.float_equal (3)
=== Instance Methods
--- Vector#ddot (1)
--- Vector#dnrm2 (0)
--- Vector#dasum (0)
--- Vector#idamax (0)
--- Vector#dcopy (1)
--- Vector#daxpy! (2)
--- Vector#daxpy (2)
--- Vector#drot (3)
--- Vector#dgemv (5)
--- Vector#dgemv! (5)
--- Vector#dtrmv (4)
--- Vector#dtrmv! (4)
--- Vector#dtrsv (4)
--- Vector#dtrsv! (4)
--- Vector#dsymv (5)
--- Vector#dsymv! (5)
--- Vector#LU_svx (2)
--- Vector#LU_svx! (2)
--- Vector#QR_svx (2)
--- Vector#QR_svx! (2)
--- Vector#QRPT_svx! (3)
--- Vector#QRPT_Rsvx! (2)
--- Vector#cholesky_svx! (1)
--- Vector#householder_transform (0)
--- Vector#householder_transform! (0)
--- Vector#householder_hv! (2)
--- Vector#householder_hv (2)
--- Vector#size (0)
--- Vector#stride (0)
--- Vector#owner (0)
--- Vector#get (1)
--- Vector#set (-1)
--- Vector#set1 (2)
--- Vector#set_all (1)
--- Vector#set_zero (0)
--- Vector#set_basis! (1)
--- Vector#set_basis (1)
--- Vector#subvector (2)
--- Vector#subvector_with_stride (3)
--- Vector#clone (0)
--- Vector#swap_elements (2)
--- Vector#reverse! (0)
--- Vector#add (1)
--- Vector#add! (1)
--- Vector#sub (1)
--- Vector#sub! (1)
--- Vector#mul (1)
--- Vector#mul! (1)
--- Vector#div (1)
--- Vector#div! (1)
--- Vector#scale (1)
--- Vector#scale! (1)
--- Vector#add_const (1)
--- Vector#add_const! (1)
--- Vector#max (0)
--- Vector#min (0)
--- Vector#minmax (0)
--- Vector#max_index (0)
--- Vector#min_index (0)
--- Vector#minmax_index (0)
--- Vector#isnull (0)
--- Vector#fwrite (1)
--- Vector#fread (1)
--- Vector#fprintf (2)
--- Vector#printf (1)
--- Vector#fscanf (1)
--- Vector#to_a (0)
--- Vector#each (0)
--- Vector#print (0)
--- Vector#inspect (0)
--- Vector#equal (1)
--- Vector#_dump_data (0)
--- Vector#_load_data (1)
--- Vector#to_complex (0)
=== Module Functions
--- Vector#memcpy (2)
--- Vector#swap (2)
== ((:Vector_complex:))
=== Superclass
(({Object}))
=== Class Methods
--- Vector_complex.new (1)
--- Vector_complex.new2 (-1)
--- Vector_complex.alloc (1)
--- Vector_complex.float_equal (3)
=== Instance Methods
--- Vector_complex#zdotu (1)
--- Vector_complex#zdotc (1)
--- Vector_complex#dznrm2 (0)
--- Vector_complex#dzasum (0)
--- Vector_complex#izamax (0)
--- Vector_complex#zcopy (1)
--- Vector_complex#zaxpy! (2)
--- Vector_complex#zaxpy (2)
--- Vector_complex#zgemv (5)
--- Vector_complex#zgemv! (5)
--- Vector_complex#ztrmv (4)
--- Vector_complex#ztrmv! (4)
--- Vector_complex#ztrsv (4)
--- Vector_complex#ztrsv! (4)
--- Vector_complex#zhemv (5)
--- Vector_complex#zhemv! (5)
--- Vector_complex#LU_svx (2)
--- Vector_complex#LU_svx! (2)
--- Vector_complex#householder_transform (0)
--- Vector_complex#householder_transform! (0)
--- Vector_complex#householder_hv! (2)
--- Vector_complex#householder_hv (2)
--- Vector_complex#size (0)
--- Vector_complex#stride (0)
--- Vector_complex#owner (0)
--- Vector_complex#get (1)
--- Vector_complex#set (-1)
--- Vector_complex#real (0)
--- Vector_complex#imag (0)
--- Vector_complex#set1 (2)
--- Vector_complex#set_all (1)
--- Vector_complex#set_zero (0)
--- Vector_complex#set_basis! (1)
--- Vector_complex#set_basis (1)
--- Vector_complex#subvector (2)
--- Vector_complex#subvector_with_stride (3)
--- Vector_complex#clone (0)
--- Vector_complex#swap_elements (2)
--- Vector_complex#reverse! (0)
--- Vector_complex#add (1)
--- Vector_complex#add! (1)
--- Vector_complex#sub (1)
--- Vector_complex#sub! (1)
--- Vector_complex#scale (1)
--- Vector_complex#scale! (1)
--- Vector_complex#add_const (1)
--- Vector_complex#add_const! (1)
--- Vector_complex#isnull (0)
--- Vector_complex#fwrite (1)
--- Vector_complex#fread (1)
--- Vector_complex#fprintf (2)
--- Vector_complex#printf (1)
--- Vector_complex#fscanf (1)
--- Vector_complex#to_a (0)
--- Vector_complex#each (0)
--- Vector_complex#print (0)
--- Vector_complex#inspect (0)
--- Vector_complex#equal (1)
--- Vector_complex#_dump_data (0)
--- Vector_complex#_load_data (1)
=== Module Functions
--- Vector_complex#memcpy (2)
--- Vector_complex#swap (2)
=end
=begin
Documentation generated with (({rude4c}))
=end
