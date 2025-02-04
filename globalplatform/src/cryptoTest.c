/*  Copyright (c) 2019, Karsten Ohme
 *  This file is part of GlobalPlatform.
 *
 *  GlobalPlatform is free software, 0x you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  GlobalPlatform is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with GlobalPlatform.  If not, see <http, 0x//www.gnu.org/licenses/>.
 */
#include <stdlib.h>
#include <setjmp.h>
#include <stdio.h>
#include <string.h>
#include <cmocka.h>
#include "crypto.h"
#include "testUtil.h"

TCHAR *exec_path;

#ifndef WIN32
#define _tcsrchr strrchr
#endif

static void test_aes_cmac_128(void **state) {
	OPGP_ERROR_STATUS status;
	// examples from https, 0x//nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-38b.pdf
	BYTE message1[] = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a};
	BYTE message2[] = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
	0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
	0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11};


	BYTE key[] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
	BYTE expected_mac_1[] = {0x07, 0x0a, 0x16, 0xb4, 0x6b, 0x4d, 0x41, 0x44, 0xf7, 0x9b, 0xdd, 0x9d, 0xd0, 0x4a, 0x28, 0x7c};
	BYTE expected_mac_2[] = {0xdf, 0xa6, 0x67, 0x47, 0xde, 0x9a, 0xe6, 0x30, 0x30, 0xca, 0x32, 0x61, 0x14, 0x97, 0xc8, 0x27};
	BYTE mac[16];

	status = calculate_CMAC_aes(key, sizeof(key), message1, sizeof(message1), NULL, mac);
	assert_int_equal(status.errorStatus, OPGP_ERROR_STATUS_SUCCESS);
	assert_memory_equal(mac, expected_mac_1, 16);

	status = calculate_CMAC_aes(key, sizeof(key), message2, sizeof(message2), NULL, mac);
	assert_int_equal(status.errorStatus, OPGP_ERROR_STATUS_SUCCESS);
	assert_memory_equal(mac, expected_mac_2, 16);
}

static void test_aes_cmac_192(void **state) {
	OPGP_ERROR_STATUS status;
	// examples from https, 0x//nvlpubs.nist.gov/nistpubs/Legacy/SP/nistspecialpublication800-38b.pdf
	BYTE message1[] = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a};
	BYTE message2[] = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a,
	0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51,
	0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11};


	BYTE key[] = {0x8e, 0x73, 0xb0, 0xf7, 0xda, 0x0e, 0x64, 0x52, 0xc8, 0x10, 0xf3, 0x2b, 0x80, 0x90, 0x79, 0xe5,
			0x62, 0xf8, 0xea, 0xd2, 0x52, 0x2c, 0x6b, 0x7b};
	BYTE expected_mac_1[] = {0x9e, 0x99, 0xa7, 0xbf, 0x31, 0xe7, 0x10, 0x90, 0x06, 0x62, 0xf6, 0x5e, 0x61, 0x7c, 0x51, 0x84};
	BYTE expected_mac_2[] = {0x8a, 0x1d, 0xe5, 0xbe, 0x2e, 0xb3, 0x1a, 0xad, 0x08, 0x9a, 0x82, 0xe6, 0xee, 0x90, 0x8b, 0x0e};
	BYTE mac[16];

	status = calculate_CMAC_aes(key, sizeof(key), message1, sizeof(message1), NULL, mac);
	assert_int_equal(status.errorStatus, OPGP_ERROR_STATUS_SUCCESS);
	assert_memory_equal(mac, expected_mac_1, 16);

	status = calculate_CMAC_aes(key, sizeof(key), message2, sizeof(message2), NULL, mac);
	assert_int_equal(status.errorStatus, OPGP_ERROR_STATUS_SUCCESS);
	assert_memory_equal(mac, expected_mac_2, 16);
}

static void test_read_public_rsa_key(void **state) {
	OPGP_ERROR_STATUS status;
	TCHAR filename_buffer[2048];
	BYTE n[128];
	LONG e = 0;
	BYTE expected_rsa_n[] = {
			 0xc6, 0x3f, 0xaf, 0x41, 0xc3, 0x43, 0xd4, 0xfa, 0xf9, 0x65, 0xe0, 0xdd, 0x32, 0xf3,
			 0xb2, 0xc5, 0xa5, 0xac, 0x31, 0x9f, 0x95, 0x46, 0x4f, 0x25, 0xf8, 0x83, 0xe2, 0xbd, 0x5b,
			 0x32, 0x9f, 0x93, 0xc0, 0xbc, 0xf4, 0x71, 0xfb, 0x66, 0x38, 0xdb, 0xd2, 0xb4, 0x5e, 0xcf,
			 0x55, 0x23, 0x4d, 0x8c, 0xd7, 0x18, 0xe7, 0x07, 0xe7, 0xca, 0x3e, 0x19, 0x1d, 0xad, 0x55,
			 0x23, 0x61, 0xf9, 0xd4, 0x22, 0x1d, 0x80, 0x04, 0x0b, 0xa5, 0x0f, 0xac, 0x08, 0xa5, 0xc0,
			 0x13, 0x7e, 0xa4, 0xa7, 0x57, 0xad, 0x2e, 0x5b, 0x19, 0x13, 0x6b, 0xeb, 0x69, 0xf0, 0xd7,
			 0x07, 0xfe, 0x16, 0x93, 0x75, 0x4c, 0x5a, 0xbe, 0xbf, 0xea, 0x59, 0xa6, 0x88, 0xa6, 0x35,
			 0x60, 0x3e, 0xc8, 0x49, 0xb2, 0x83, 0x1b, 0xa6, 0xbe, 0x26, 0xaf, 0x63, 0x7f, 0x6d, 0x7d,
			 0x1d, 0x63, 0x8c, 0xe7, 0x99, 0xca, 0x02, 0xb4, 0x57
			};
	LONG expected_rsa_e = 0x010001L;
	_sntprintf(filename_buffer, sizeof(filename_buffer), _T("%s/%s"), exec_path, _T("rsa_pub_key_test.pem"));
	status = read_public_rsa_key(filename_buffer, "password", n, &e);
	assert_int_equal(status.errorStatus, OPGP_ERROR_STATUS_SUCCESS);
	assert_memory_equal(n, expected_rsa_n, sizeof(n));
	assert_int_equal(e, expected_rsa_e);
}

static int setup(void **state) {
	return 0;
}

int main(int argc, TCHAR *argv[]) {
	TCHAR *end_path;
	exec_path = argv[0];
	end_path = _tcsrchr(exec_path, _T('/'));
	exec_path[_tcslen(exec_path) - _tcslen(end_path)] = _T('\0');
	const struct CMUnitTest tests[] = {
			cmocka_unit_test(test_aes_cmac_128),
			cmocka_unit_test(test_aes_cmac_192),
			cmocka_unit_test(test_read_public_rsa_key),
	};
	return cmocka_run_group_tests_name("crypto", tests, setup, NULL);
}
