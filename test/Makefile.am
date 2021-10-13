LDADD = -lmeasuresuite
AM_LDFLAGS = -L$(top_srcdir)/src
AM_CPPFLAGS = -I$(top_srcdir)/src/

# To add a new test, add it to var "check_PROGRAMS" and if needed another built file to test_libs. 
# Then add a block new_test_SOURCES and its needed files: usually new_test.c and maybe libnewlib.so
# If you have a libnewlib.so, you may add a target to create these. Those should have "Makefile" as a prereq to make sure it gets rebuilt.

check_PROGRAMS = no_checkfunctions_so \
				 test_add_two_numbers \
				 test_plus1 \
				 test_batchsize \
				 test_wrongResult \
				 test_invalid_instruction \
				 test_six_param \
				 test_array_width_three \
				 test_array_width_three_mul \
				 test_array_width_three_add_pair \
				 test_array_width_five_add_pair \
				 test_chunking \
				 test_checker \
				 test_evaluator \
				 test_p224_mul \
				 test_p224_sq \
				 test_p256_mul \
				 test_p256_sq \
				 test_p384_mul \
				 test_p384_sq \
				 test_p434_mul \
				 test_p434_sq \
				 test_p521_mul \
				 test_p521_sq \
				 test_poly1305_mul \
				 test_poly1305_sq \
				 test_curve25519_mul \
				 test_curve25519_sq
# In case you want a test to fail for the suite to pass, add the test here
XFAIL_TESTS= no_checkfunctions_so

#info on BUILT_SOURCES: https://www.gnu.org/software/automake/manual/html_node/Built-Sources-Example.html
BUILT_SOURCES = libtest_lib.so \
				libcheckfunctions-clang-AMD.so

CLEANFILES = $(BUILT_SOURCES) .gdb_history

TESTS = $(check_PROGRAMS)

test_evaluator_SOURCES = test_evaluator.c \
						 libtest_lib.so

no_checkfunctions_so_SOURCES = no_checkfunctions_so.c \
					           test_helper.c

#libtest_lib
test_add_two_numbers_SOURCES = test_add_two_numbers.c \
							   test_helper.c \
							   libtest_lib.so

test_plus1_SOURCES = test_plus1.c \
					 test_helper.c \
					 libtest_lib.so

test_batchsize_SOURCES = test_batchsize.c \
					 	 test_helper.c \
					 	 libtest_lib.so

test_wrongResult_SOURCES = test_wrongResult.c \
						   test_helper.c \
						   libtest_lib.so

test_invalid_instruction_SOURCES = test_invalid_instruction.c \
								   test_helper.c \
								   libtest_lib.so

test_six_param_SOURCES = test_six_param.c \
						 test_helper.c \
						 libtest_lib.so

test_array_width_three_SOURCES = test_array_width_three.c \
								 test_helper.c \
								 libtest_lib.so

test_array_width_three_mul_SOURCES = test_array_width_three_mul.c \
									 test_helper.c \
									 libtest_lib.so

test_array_width_three_add_pair_SOURCES = test_array_width_three_add_pair.c \
									 test_helper.c \
									 libtest_lib.so

test_array_width_five_add_pair_SOURCES = test_array_width_five_add_pair.c \
									 test_helper.c \
									 libtest_lib.so
test_chunking_SOURCES = test_chunking.c \
						test_helper.c \
						libtest_lib.so

#libcheckfunctions
test_curve25519_mul_SOURCES = test_curve25519_mul.c \
							  test_helper.c \
							  libcheckfunctions-clang-AMD.so

test_curve25519_sq_SOURCES = test_curve25519_sq.c \
							 test_helper.c \
							 libcheckfunctions-clang-AMD.so

test_p224_mul_SOURCES = test_p224_mul.c \
                       test_helper.c \
                       libcheckfunctions-clang-AMD.so

test_p224_sq_SOURCES = test_p224_sq.c \
                       test_helper.c \
                       libcheckfunctions-clang-AMD.so

test_p256_mul_SOURCES = test_p256_mul.c \
                       test_helper.c \
                       libcheckfunctions-clang-AMD.so

test_p256_sq_SOURCES = test_p256_sq.c \
                       test_helper.c \
                       libcheckfunctions-clang-AMD.so

test_p384_mul_SOURCES = test_p384_mul.c \
                       test_helper.c \
                       libcheckfunctions-clang-AMD.so

test_p384_sq_SOURCES = test_p384_sq.c \
                       test_helper.c \
                       libcheckfunctions-clang-AMD.so

test_p434_mul_SOURCES = test_p434_mul.c \
                       test_helper.c \
                       libcheckfunctions-clang-AMD.so

test_p434_sq_SOURCES = test_p434_sq.c \
                       test_helper.c \
                       libcheckfunctions-clang-AMD.so

test_p521_mul_SOURCES = test_p521_mul.c \
                       test_helper.c \
                       libcheckfunctions-clang-AMD.so

test_p521_sq_SOURCES = test_p521_sq.c \
                       test_helper.c \
                       libcheckfunctions-clang-AMD.so

test_poly1305_mul_SOURCES = test_poly1305_mul.c \
                       test_helper.c \
                       libcheckfunctions-clang-AMD.so

test_poly1305_sq_SOURCES = test_poly1305_sq.c \
                       test_helper.c \
                       libcheckfunctions-clang-AMD.so
 

libtest_lib.so: test_lib.c Makefile
	$(CC) -fPIC -shared test_lib.c  -o ${@}

libcheckfunctions-clang-AMD.so: ./all_fiat.c Makefile
	$(CC) -fPIC -shared ./all_fiat.c -o ${@}

