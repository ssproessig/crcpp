/*
 * $Id$
 * $Date$
 * 
 * CRCTest.cpp: implementation of the CRCTest class.
 * 
 * Copyright (c) 2003 INTEC International GmbH, Hechingen, Germany
 * Author: Adrian Weiler
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 * 
 */

#include "CRCTest.h"

#include "crc.h"
#include <iostream>

CPPUNIT_TEST_SUITE_REGISTRATION( CRCTest );

extern "C" void sTestCRC() {}; // Allow to be referenced by unit test (reference "_sTestCRC")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRCTest::CRCTest()
{
}

CRCTest::~CRCTest()
{

}

void CRCTest::setUp()
{
	testPattern = "123456789";
}

void CRCTest::tearDown()
{
}

void CRCTest::testNative ()
{
	std::cout << "Testing CRC in native bit order...";

	CRC<Poly16> CRCx (0x1021);
	CRCStream <Poly16> cs (CRCx);
	Poly16 result;

	// Basic test
	cs << testPattern;
	CPPUNIT_ASSERT (cs.CRC() == 0xD64E);
	CPPUNIT_ASSERT (cs.result() == std::string("\xD6\x4E"));

	// Test the gen/check routine
	sResult = cs.gen(testPattern);
	CPPUNIT_ASSERT (sResult == std::string("\xD6\x4E"));
	CPPUNIT_ASSERT (cs.check(testPattern+sResult));

	std::cout << "OK." << std::endl;
}

void CRCTest::testNetwork ()
{
	std::cout << "Testing CRC-CCITT...";

	CRC<Poly16N> CRC_CCITT (0x8408);
	CRCStream <Poly16N> cs (CRC_CCITT);
	Poly16N result;

	// Basic test
	cs << testPattern;
	CPPUNIT_ASSERT (cs.result() == std::string("\x6E\x90"));

	char const * testData = "\x01\x73";
	cs.reset();
	cs << testData;
	result = cs.CRC();

	cs.reset();

	cs << testData << result;

	CPPUNIT_ASSERT (cs.good());

	// Test the gen/check routine
	sResult = cs.gen(testPattern);
	CPPUNIT_ASSERT (sResult == std::string("\x6E\x90"));
	CPPUNIT_ASSERT (cs.check(testPattern+sResult));

	sResult = cs.gen(testData);
	CPPUNIT_ASSERT (sResult == std::string("\x83\x57"));
	CPPUNIT_ASSERT (cs.check(std::string(testData)+sResult));
	std::cout << "OK." << std::endl;
}

void CRCTest::testEthernet ()
{
	std::cout << "Testing Ethernet CRC...";

	CRC<Poly32N> CRC_ETHER (0xEDB88320);
	CRCStream <Poly32N> cs (CRC_ETHER);
	Poly32N result;

	sResult = cs.gen(testPattern);
	result = cs.CRC();

	// Test data recorded from real ethernet, including CRC
	std::string sTestData ("\x00\x00\x1a\x18\x26\xb4\x00\x50\x04\x6e\x8b\x4f\x08\x00\x45\x00\x00\x3c\xa6\x42\x00\x00\x80\x01\xec\x5e\xc0\xa8\x13\x07\xc0\xa8\x13\xc8\x08\x00\x48\x5c\x02\x00\x03\x00\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f\x70\x71\x72\x73\x74\x75\x76\x77\x61\x62\x63\x64\x65\x66\x67\x68\x69\x2c\x71\xa1\x0c", 78);

	CPPUNIT_ASSERT (cs.check(sTestData));

	std::cout << "OK." << std::endl;
}