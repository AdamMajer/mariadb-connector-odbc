/*
  Copyright (c) 2001, 2012, Oracle and/or its affiliates. All rights reserved.
                2013, 2016 MariaDB Corporation AB

  The MySQL Connector/ODBC is licensed under the terms of the GPLv2
  <http://www.gnu.org/licenses/old-licenses/gpl-2.0.html>, like most
  MySQL Connectors. There are special exceptions to the terms and
  conditions of the GPLv2 as it is applied to this software, see the
  FLOSS License Exception
  <http://www.mysql.com/about/legal/licensing/foss-exception.html>.
  
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published
  by the Free Software Foundation; version 2 of the License.
  
  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
  for more details.
  
  You should have received a copy of the GNU General Public License along
  with this program; if not, write to the Free Software Foundation, Inc.,
  51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA
*/

#include "tap.h"

ODBC_TEST(test_CONO1)
{
  /* check SQLColumns with ANSI_QUOTES on and off */
  SQLRETURN ret;
  SQLLEN rowCount;
  SQLCHAR *create_table= "CREATE TABLE cono1 (InitialStartDateTime datetime NOT NULL,  TicketId int(11) NOT NULL AUTO_INCREMENT,  CallCount int(11) NOT NULL DEFAULT '1',  CalledNumber varchar(30) DEFAULT NULL,  CallingNumber varchar(30) DEFAULT NULL,  CallType tinyint(3) unsigned DEFAULT NULL,  ChargeUnits smallint(6) DEFAULT NULL,  NetworkAndTrunkNode int(11) DEFAULT NULL,  TrunkGroupIdentity varchar(10) DEFAULT NULL,  EntityId int(11) DEFAULT NULL,  PersonalOrBusiness tinyint(3) unsigned DEFAULT NULL,   WaitingDuration smallint(6) DEFAULT '0',  EffectiveCallDuration int(11) DEFAULT NULL,  ComType tinyint(3) unsigned DEFAULT NULL,  CostInfo double DEFAULT NULL,  InitialDialledNumber varchar(30) DEFAULT NULL,  Carrier varchar(5) DEFAULT NULL,  UserToUserVolume smallint(6) DEFAULT '0',  StartDateTime datetime DEFAULT NULL,  Duration int(11) DEFAULT NULL,  RedirectedCallIndicator tinyint(3) unsigned DEFAULT NULL,  Subaddress varchar(20) DEFAULT NULL,  HighLevelComp tinyint(3) unsigned DEFAULT NULL,  CostType tinyint(3) unsigned DEFAULT NULL,  TrunkIdentity smallint(6) DEFAULT NULL,  SpecificChargeInfo char(7) DEFAULT NULL,  BearerCapability tinyint(3) unsigned DEFAULT NULL,  DataVolume int(11) DEFAULT NULL,  AdditionalEntityId int(11) DEFAULT NULL,  FirstCarrierCost double NOT NULL,  FirstCarrierCostT double DEFAULT NULL,  SecondCarrierCost double NOT NULL,  SecondCarrierCostT double DEFAULT NULL,  FacilityCost double NOT NULL,  FacilityCostT double DEFAULT NULL,  FacturedCost double DEFAULT NULL,  FacturedCostT double DEFAULT NULL,  SubscriptionCost double NOT NULL DEFAULT '0',  SubscriptionCostT double DEFAULT NULL,  FirstCarrierId int(11) DEFAULT NULL,  SecondCarrierId int(11) DEFAULT NULL,  FirstCarrierDirectionId int(11) DEFAULT NULL,  SecondCarrierDirectionId int(11) DEFAULT NULL,  FirstCarrierCcnId int(11) DEFAULT NULL,  SecondCarrierCcnId int(11) DEFAULT NULL,  ActingExtensionNumber varchar(30) DEFAULT NULL,  TransitTrunkGroupIdentity varchar(5) DEFAULT NULL,  NodeTimeOffset smallint(6) DEFAULT NULL,  ExternFacilities binary(5) DEFAULT NULL,  InternFacilities binary(5) DEFAULT NULL,  TicketOrigin tinyint(3) unsigned DEFAULT '0',  TimeDlt int(11) DEFAULT NULL,  PRIMARY KEY (TicketId),  UNIQUE KEY IX_Ticket (TicketId),  KEY IX2_Ticket (EntityId),  KEY IX3_Ticket (InitialStartDateTime),  KEY IX4_Ticket (StartDateTime))";

  CHECK_STMT_RC(Stmt, SQLExecDirect(Stmt, "SET SQL_MODE='ANSI_QUOTES'", SQL_NTS));
  CHECK_STMT_RC(Stmt, SQLExecDirect(Stmt, "DROP TABLE IF EXISTS cono1", SQL_NTS));
  CHECK_STMT_RC(Stmt, SQLExecDirect(Stmt, create_table, SQL_NTS));
  

  ret= SQLColumns(Stmt, NULL, 0, NULL, 0, "cono1", SQL_NTS, NULL, 0);
  if (!SQL_SUCCEEDED(ret))
    return FAIL;

  SQLRowCount(Stmt, &rowCount);
  diag("row_count: %u", rowCount);

  CHECK_STMT_RC(Stmt, SQLExecDirect(Stmt, "SET SQL_MODE=''", SQL_NTS));

  ret= SQLColumns(Stmt, NULL, 0, NULL, 0, "cono1", SQL_NTS, NULL, 0);
  if (!SQL_SUCCEEDED(ret))
    return FAIL;

  SQLRowCount(Stmt, &rowCount);
  diag("row_count: %u", rowCount);

  return OK;
}

ODBC_TEST(test_CONO3)
{
  int i= 0;

  OK_SIMPLE_STMT(Stmt, "SET @a:=1");
  CHECK_STMT_RC(Stmt, SQLBindCol(Stmt, 1, SQL_C_LONG, (SQLPOINTER)(SQLLEN)i, 0, NULL));

  return OK;
}


ODBC_TEST(simple_test)
{
  SQLRETURN rc= SQL_SUCCESS;

  SQLINTEGER value=3;
  SQLWCHAR Buffer[20];

  char buffer[128];

  OK_SIMPLE_STMT(Stmt, "SHOW VARIABLES LIKE 'character_set_client'");
  SQLFetch(Stmt);
  SQLGetData(Stmt, 2, SQL_CHAR, buffer, 20, NULL);
  FAIL_IF(SQLFetch(Stmt) != SQL_NO_DATA, "Eof expected");

  CHECK_STMT_RC(Stmt, SQLFreeStmt(Stmt, SQL_CLOSE));

  OK_SIMPLE_STMTW(Stmt, CW("DROP TABLE IF EXISTS smpltest"));
  OK_SIMPLE_STMTW(Stmt, CW("CREATE TABLE smpltest (a int, b varchar(25))"));
  OK_SIMPLE_STMTW(Stmt, CW("INSERT INTO smpltest VALUES (1, 'Row no 1')"));
  OK_SIMPLE_STMTW(Stmt, CW("INSERT INTO smpltest VALUES (2, 'Row no 2')"));
  
  rc= SQLPrepareW(Stmt, CW("SELECT a, b FROM smpltest"), SQL_NTS);
  rc= SQLExecute(Stmt);
  
  SQLFetch(Stmt);
  SQLGetData(Stmt, 1, SQL_C_USHORT, &value, sizeof(value), 0);
  SQLGetData(Stmt, 2, SQL_C_WCHAR, Buffer, 20, 0);
  FAIL_IF(value != 1, "Expected value=1");

  IS_WSTR(Buffer, CW("Row no 1"), 9);

  rc= SQLFetch(Stmt);
  SQLGetData(Stmt, 1, SQL_C_USHORT, &value, sizeof(value), 0);
  SQLGetData(Stmt, 2, SQL_C_WCHAR, Buffer, 20, 0);
  FAIL_IF(value != 2, "Expected value=2");
  IS_WSTR(Buffer, CW("Row no 2"), 9);

  CHECK_STMT_RC(Stmt, SQLFreeStmt(Stmt, SQL_CLOSE));
  OK_SIMPLE_STMTW(Stmt, CW("DROP TABLE IF EXISTS smpltest"));

  return OK;
}


ODBC_TEST(simple_test1)
{
  SQLLEN nRowCount;
  SQLRETURN rc;

  OK_SIMPLE_STMTW(Stmt, CW("DROP TABLE IF EXISTS t_basic, t_basic_2"));

  /* create the table 'myodbc3_demo_result' */
  OK_SIMPLE_STMTW(Stmt,
         CW("CREATE TABLE t_basic (id INT PRIMARY KEY, name VARCHAR(20))"));

  /* insert 3 rows of data */
  OK_SIMPLE_STMTW(Stmt, CW("INSERT INTO t_basic VALUES (1,'foo'),(2,'bar'),(3,'baz')"));

  /* update second row */
  OK_SIMPLE_STMTW(Stmt, CW("UPDATE t_basic SET name = 'bop' WHERE id = 2"));

  /* get the rows affected by update statement */
  rc= SQLRowCount(Stmt, &nRowCount);
  CHECK_STMT_RC(Stmt, rc);
  FAIL_IF(nRowCount != 1, "Rowcount != 1");
  
  /* delete third row */
  OK_SIMPLE_STMTW(Stmt, CW("DELETE FROM t_basic WHERE id = 3"));

  /* get the rows affected by delete statement */
  rc= SQLRowCount(Stmt, &nRowCount);
  CHECK_STMT_RC(Stmt, rc);
  FAIL_IF(nRowCount != 1, "Rowcount != 1");

  /* alter the table 't_basic' to 't_basic_2' */
  OK_SIMPLE_STMTW(Stmt, CW("ALTER TABLE t_basic RENAME t_basic_2"));

  /*
    drop the table with the original table name, and it should
    return error saying 'table not found'
  */
  ERR_SIMPLE_STMTW(Stmt, CW("DROP TABLE t_basic"));

 /* now drop the table, which is altered..*/
  OK_SIMPLE_STMTW(Stmt, CW("DROP TABLE t_basic_2"));

  return OK;
}

ODBC_TEST(select1000)
{
  SQLRETURN rc;
  SQLINTEGER num;
  SQLCHAR    szData[20];

  OK_SIMPLE_STMTW(Stmt, CW("DROP TABLE IF EXISTS t_max_select"));

  OK_SIMPLE_STMTW(Stmt, CW("CREATE TABLE t_max_select (a INT, b VARCHAR(30))"));

  rc= SQLPrepareW(Stmt, CW("INSERT INTO t_max_select VALUES (?,?)"), SQL_NTS);
  CHECK_STMT_RC(Stmt, rc);

  rc= SQLBindParameter(Stmt, 1, SQL_PARAM_INPUT, SQL_C_LONG,
                                  SQL_INTEGER, 0, 0, &num, 0, NULL);
  CHECK_STMT_RC(Stmt, rc);
  rc= SQLBindParameter(Stmt, 2, SQL_PARAM_INPUT, SQL_C_CHAR,
                                  SQL_CHAR, 0, 0, szData, sizeof(szData),
                                  NULL);
  CHECK_STMT_RC(Stmt, rc);

  for (num= 1; num <= 1000; num++)
  {
    sprintf((char *)szData, "MySQL%d", (int)num);
    rc= SQLExecute(Stmt);
    CHECK_STMT_RC(Stmt, rc);
  }

  rc= SQLFreeStmt(Stmt, SQL_RESET_PARAMS);
  rc= SQLFreeStmt(Stmt, SQL_CLOSE);

  OK_SIMPLE_STMTW(Stmt, CW("SELECT * FROM t_max_select"));
  num= 0;

  while (SQL_SUCCESS == SQLFetch(Stmt))
    num++;

  FAIL_IF(num != 1000, "Expected 1000 rows");

  rc= SQLFreeStmt(Stmt, SQL_UNBIND);
  rc= SQLFreeStmt(Stmt, SQL_CLOSE);

  OK_SIMPLE_STMTW(Stmt, CW("DROP TABLE IF EXISTS t_max_select"));

  return OK;
}

ODBC_TEST(simple_2)
{
  SQLINTEGER nRowCount= 0, nInData= 1, nOutData;
  SQLCHAR szOutData[31];
  SQLRETURN rc;

  OK_SIMPLE_STMTW(Stmt, CW("DROP TABLE IF EXISTS t_myodbc"));

  OK_SIMPLE_STMTW(Stmt, CW("CREATE TABLE t_myodbc (a INT, b VARCHAR(30))"));

  rc= SQLFreeStmt(Stmt, SQL_CLOSE);
  CHECK_STMT_RC(Stmt, rc);

  /* DIRECT INSERT */
  OK_SIMPLE_STMTW(Stmt, CW("INSERT INTO t_myodbc VALUES (10, 'direct')"));

  /* PREPARE INSERT */
  rc= SQLPrepareW(Stmt, CW("INSERT INTO t_myodbc VALUES (?, 'param')"), SQL_NTS);
  CHECK_STMT_RC(Stmt, rc);

  rc= SQLBindParameter(Stmt, 1, SQL_PARAM_INPUT, SQL_C_LONG,
                       SQL_INTEGER, 0, 0, &nInData, 0, NULL);
  CHECK_STMT_RC(Stmt, rc);

  for (nInData= 20; nInData < 100; nInData= nInData+10)
  {
    rc= SQLExecute(Stmt);
    CHECK_STMT_RC(Stmt, rc);
  }

  /* FREE THE PARAM BUFFERS */
  rc= SQLFreeStmt(Stmt, SQL_RESET_PARAMS);
  CHECK_STMT_RC(Stmt, rc);
  rc= SQLFreeStmt(Stmt, SQL_CLOSE);
  CHECK_STMT_RC(Stmt, rc);

  /* FETCH RESULT SET */
  OK_SIMPLE_STMTW(Stmt, CW("SELECT * FROM t_myodbc"));

  rc= SQLBindCol(Stmt, 1, SQL_C_LONG, &nOutData, 0, NULL);
  CHECK_STMT_RC(Stmt, rc);
  rc= SQLBindCol(Stmt, 2, SQL_C_CHAR, szOutData, sizeof(szOutData), NULL);
  CHECK_STMT_RC(Stmt, rc);

  nInData= 10;
  while (SQLFetch(Stmt) == SQL_SUCCESS)
  {
    FAIL_IF(nOutData != nInData, "OutData != InData");
    FAIL_IF(strncmp(szOutData, nRowCount++ ? "param" : "direct", 5), "");
    nInData += 10;
  }

  FAIL_IF(nRowCount != (nInData - 10) / 10, "");

  /* FREE THE OUTPUT BUFFERS */
  rc= SQLFreeStmt(Stmt, SQL_UNBIND);
  CHECK_STMT_RC(Stmt, rc);
  rc= SQLFreeStmt(Stmt, SQL_CLOSE);
  CHECK_STMT_RC(Stmt, rc);
  OK_SIMPLE_STMTW(Stmt, CW("DROP TABLE IF EXISTS t_myodbc"));

  return OK;
}

ODBC_TEST(test_reconnect)
{
  SQLHDBC hdbc1;
  SQLRETURN rc;
  int i;
  SQLWCHAR dsn[256],
           username[64],
           passwd[64];

  for (i= 0; i < 10; i++)
  {
    rc= SQLAllocHandle(SQL_HANDLE_DBC, Env, &hdbc1);
    CHECK_ENV_RC(Env, rc);
    
    rc= SQLConnectW(hdbc1, latin_as_sqlwchar(my_dsn, dsn), SQL_NTS, latin_as_sqlwchar(my_uid, username), SQL_NTS,
                   latin_as_sqlwchar(my_pwd, passwd), SQL_NTS);
    CHECK_DBC_RC(hdbc1, rc);
    rc= SQLDisconnect(hdbc1);
    CHECK_DBC_RC(hdbc1, rc);
    rc= SQLFreeHandle(SQL_HANDLE_DBC, hdbc1);
    if (!SQL_SUCCEEDED(rc))
      CHECK_DBC_RC(hdbc1, rc);
  }

  return OK;
}

ODBC_TEST(t_disconnect)
{
  SQLHDBC hdbc1;
  SQLRETURN rc;
  int i;
  SQLHSTMT hstmt;
  SQLWCHAR dsn[256],
           username[64],
           passwd[64];
  rc= SQLAllocHandle(SQL_HANDLE_DBC, Env, &hdbc1);
    CHECK_ENV_RC(Env, rc);
  rc= SQLConnectW(hdbc1, latin_as_sqlwchar(my_dsn, dsn), SQL_NTS, latin_as_sqlwchar(my_uid, username), SQL_NTS,
                   latin_as_sqlwchar(my_pwd, passwd), SQL_NTS);
  CHECK_DBC_RC(hdbc1, rc);

  for (i=0; i < 100; i++)
  {
    rc= SQLAllocHandle(SQL_HANDLE_STMT, hdbc1, &hstmt); 
    CHECK_DBC_RC(hdbc1, rc);

    rc= SQLSetStmtAttr(hstmt, SQL_ATTR_CURSOR_TYPE, SQL_CURSOR_FORWARD_ONLY, 0);
    CHECK_STMT_RC(hstmt, rc);
    rc= SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    CHECK_STMT_RC(hstmt, rc);
    rc= SQLEndTran(SQL_HANDLE_DBC, hdbc1, 1);
    CHECK_DBC_RC(hdbc1, rc);
  }
  rc= SQLDisconnect(hdbc1);
  CHECK_DBC_RC(hdbc1, rc);
  rc= SQLFreeHandle(SQL_HANDLE_DBC, hdbc1);
  if (!SQL_SUCCEEDED(rc))
    CHECK_DBC_RC(hdbc1, rc);
  
  return OK;
}

ODBC_TEST(bug19823)
{
  SQLHDBC Hdbc;

  SQLUINTEGER timeout;
  SQLRETURN rc;

  rc= SQLAllocHandle(SQL_HANDLE_DBC, Env, &Hdbc);
  CHECK_ENV_RC(Env, rc);

  rc= SQLSetConnectAttr(Hdbc, SQL_ATTR_LOGIN_TIMEOUT,
                              (SQLPOINTER)9, 0);
  rc= SQLSetConnectAttr(Hdbc, SQL_ATTR_CONNECTION_TIMEOUT,
                              (SQLPOINTER)10, 0);
  CHECK_DBC_RC(Hdbc, rc);

  rc= SQLConnect(Hdbc, my_dsn, SQL_NTS, my_uid, SQL_NTS,
                   my_pwd, SQL_NTS);
  CHECK_DBC_RC(Hdbc, rc);

  rc= SQLGetConnectAttr(Hdbc, SQL_ATTR_LOGIN_TIMEOUT,
                               &timeout, 0, NULL);
  CHECK_DBC_RC(Hdbc, rc);
  FAIL_IF(timeout != 9, "Login_timeout != 9");

  rc= SQLGetConnectAttr(Hdbc, SQL_ATTR_CONNECTION_TIMEOUT,
                               &timeout, 0, NULL);

  /* Since connection timeout is not supported, the value
     must be 0 */
  CHECK_DBC_RC(Hdbc, rc);
  FAIL_IF(timeout != 0, "connection_timeout != 0");

  rc= SQLDisconnect(Hdbc);
  rc= SQLFreeHandle(SQL_HANDLE_DBC, Hdbc);

  return OK;
}

ODBC_TEST(t_basic)
{
  SQLINTEGER nRowCount= 0, nInData= 1, nOutData;
  SQLCHAR szOutData[31];
  SQLRETURN rc;

 
  OK_SIMPLE_STMT(Stmt, "DROP TABLE IF EXISTS t_myodbc");

  OK_SIMPLE_STMT(Stmt, "CREATE TABLE t_myodbc (a INT, b VARCHAR(30))");

  rc= SQLFreeStmt(Stmt, SQL_CLOSE);
  CHECK_HANDLE_RC(SQL_HANDLE_STMT, Stmt, rc);

  /* DIRECT INSERT */
  OK_SIMPLE_STMT(Stmt, "INSERT INTO t_myodbc VALUES (10, 'direct')");

  /* PREPARE INSERT */
  CHECK_HANDLE_RC(SQL_HANDLE_STMT, Stmt, SQLPrepare(Stmt,
                            (SQLCHAR *)
                            "INSERT INTO t_myodbc VALUES (?, 'param')",
                            SQL_NTS));

  CHECK_HANDLE_RC(SQL_HANDLE_STMT, Stmt, SQLBindParameter(Stmt, 1, SQL_PARAM_INPUT, SQL_C_LONG,
                                  SQL_INTEGER, 0, 0, &nInData, 0, NULL));

  for (nInData= 20; nInData < 100; nInData= nInData+10)
  {
    CHECK_HANDLE_RC(SQL_HANDLE_STMT, Stmt, SQLExecute(Stmt));
  }

  /* FREE THE PARAM BUFFERS */
   CHECK_HANDLE_RC(SQL_HANDLE_STMT, Stmt, SQLFreeStmt(Stmt, SQL_RESET_PARAMS));
   CHECK_HANDLE_RC(SQL_HANDLE_STMT, Stmt, SQLFreeStmt(Stmt, SQL_CLOSE));

  /* FETCH RESULT SET */
  OK_SIMPLE_STMT(Stmt, "SELECT * FROM t_myodbc");

  CHECK_HANDLE_RC(SQL_HANDLE_STMT, Stmt, SQLBindCol(Stmt, 1, SQL_C_LONG, &nOutData, 0, NULL));
  CHECK_HANDLE_RC(SQL_HANDLE_STMT, Stmt, SQLBindCol(Stmt, 2, SQL_C_CHAR, szOutData, sizeof(szOutData),
                            NULL));

  nInData= 10;
  while (SQLFetch(Stmt) == SQL_SUCCESS)
  {
    FAIL_IF(nOutData != nInData, "in != out");
    FAIL_IF(strncmp(szOutData, nRowCount++ ? "param" : "direct", 5) != 0, "comparison failed");
    nInData += 10;
  }

  FAIL_IF(nRowCount != (nInData - 10) / 10, "comparison failed");

  /* FREE THE OUTPUT BUFFERS */
  CHECK_HANDLE_RC(SQL_HANDLE_STMT, Stmt, SQLFreeStmt(Stmt, SQL_UNBIND));
  CHECK_HANDLE_RC(SQL_HANDLE_STMT, Stmt, SQLFreeStmt(Stmt, SQL_CLOSE));

  OK_SIMPLE_STMT(Stmt, "DROP TABLE IF EXISTS t_myodbc");

  return OK;
}

ODBC_TEST(t_reconnect)
{
  SQLHDBC hdbc1;
  long i;
  SQLRETURN ret;

  for (i= 0; i < 10; i++)
  {
    ret= SQLAllocConnect(Env, &hdbc1);
    CHECK_HANDLE_RC(SQL_HANDLE_ENV, Env, ret);
    CHECK_HANDLE_RC(SQL_HANDLE_DBC, hdbc1, SQLConnect(hdbc1, my_dsn, SQL_NTS, my_uid, SQL_NTS,
                             my_pwd, SQL_NTS));
    CHECK_HANDLE_RC(SQL_HANDLE_DBC, hdbc1, SQLDisconnect(hdbc1));
    CHECK_HANDLE_RC(SQL_HANDLE_DBC, hdbc1, SQLFreeConnect(hdbc1));
  }

  return OK;
}

int GetIntVal(SQLHANDLE hStmt, SQLINTEGER Column)
{
  int Value;

  CHECK_STMT_RC(hStmt, SQLGetData(hStmt, (SQLUSMALLINT)Column, SQL_C_LONG, &Value, 0, NULL));
  printf("Value: %d\n", Value);
  return Value;
}

ODBC_TEST(charset_utf8)
{
  HDBC hdbc1;
  HSTMT hstmt1;
  SQLCHAR conn[512], conn_out[512];
  SQLLEN len;
  SQLSMALLINT conn_out_len;
  SQLINTEGER str_size;
  SQLWCHAR wc[20];

  /**
   Bug #19345: Table column length multiplies on size session character set
  */
  OK_SIMPLE_STMT(Stmt, "DROP TABLE IF EXISTS t_bug19345");
  OK_SIMPLE_STMT(Stmt, "CREATE TABLE t_bug19345 (a VARCHAR(10), b VARBINARY(10)) CHARACTER SET Utf8");
  OK_SIMPLE_STMT(Stmt, "INSERT INTO t_bug19345 VALUES ('abc','def')");

  CHECK_STMT_RC(Stmt, SQLFreeStmt(Stmt, SQL_CLOSE));

  sprintf((char *)conn, "DSN=%s;UID=%s;PWD=%s;CHARSET=utf8",
         my_dsn, my_uid, my_pwd);
  
  CHECK_ENV_RC(Env, SQLAllocHandle(SQL_HANDLE_DBC, Env, &hdbc1));

  CHECK_DBC_RC(hdbc1, SQLDriverConnect(hdbc1, NULL, conn, sizeof(conn), conn_out,
                                 sizeof(conn_out), &conn_out_len,
                                 SQL_DRIVER_NOPROMPT));
  CHECK_DBC_RC(hdbc1, SQLAllocStmt(hdbc1, &hstmt1));

  CHECK_STMT_RC(hstmt1, SQLExecDirect(hstmt1, "SELECT _latin1 0x73E36F207061756C6F", SQL_NTS));

  CHECK_STMT_RC(hstmt1, SQLFetch(hstmt1));

  SQLGetData(hstmt1, 1, SQL_CHAR, conn_out, 512, NULL);
  FAIL_IF(strncmp(conn_out, "s\xC3\xA3o paulo", 10) != 0, "Comparison mismatch");
    
  FAIL_IF(SQLFetch(hstmt1) != SQL_NO_DATA, "End of result expected");

  CHECK_STMT_RC(hstmt1, SQLFreeStmt(hstmt1, SQL_CLOSE));

  CHECK_STMT_RC(hstmt1, SQLColumns(hstmt1, (SQLCHAR *)my_schema, SQL_NTS, NULL, 0,
                             (SQLCHAR *)"t_bug19345", SQL_NTS,
                             (SQLCHAR *)"%", 1));

  CHECK_STMT_RC(hstmt1, SQLFetch(hstmt1));
  FAIL_IF(GetIntVal(hstmt1, 7) != 10, "Expected length 10");
  str_size= GetIntVal(hstmt1, 8);
  /* utf8 mbmaxlen = 3 in libmysql before MySQL 6.0 */
  
  if (str_size == 30)
  {
    FAIL_IF(GetIntVal(hstmt1, 16) != 30, "Comparison failed");
  }
  else
  {
    FAIL_IF(str_size != 40, "Comparison failed");
    FAIL_IF(GetIntVal(hstmt1, 16) != 40, "Comparison failed");
  }

  CHECK_STMT_RC(hstmt1, SQLFetch(hstmt1));
  FAIL_IF(GetIntVal(hstmt1, 7) != 10, "Comparison failed");
  FAIL_IF(GetIntVal(hstmt1, 8) != 10, "Comparison failed");
  FAIL_IF(GetIntVal(hstmt1, 16) != 10, "Comparison failed");
    
  CHECK_STMT_RC(hstmt1, SQLFreeStmt(hstmt1, SQL_CLOSE));

  OK_SIMPLE_STMT(hstmt1, "SELECT _big5 0xA4A4");

  CHECK_STMT_RC(hstmt1, SQLFetch(hstmt1));

  CHECK_STMT_RC(hstmt1, SQLGetData(hstmt1, 1, SQL_C_CHAR, conn, 2, &len));
  FAIL_IF(conn[0] != 0xE4, "Comparison failed");
  FAIL_IF(len != 3, "Comparison failed");
  
  CHECK_STMT_RC(hstmt1, SQLGetData(hstmt1, 1, SQL_C_CHAR, conn, 2, &len));
  FAIL_IF(conn[0] != 0xB8, "Comparison failed");
  FAIL_IF(len != 2, "Comparison failed");
  
  CHECK_STMT_RC(hstmt1, SQLGetData(hstmt1, 1, SQL_C_CHAR, conn, 2, &len));
  FAIL_IF(conn[0] != 0xAD, "Comparison failed");
  FAIL_IF(len != 1, "Comparison failed");
  
  FAIL_IF(SQLGetData(hstmt1, 1, SQL_C_CHAR, conn, 2, &len) != SQL_NO_DATA_FOUND, "Expected SQL_NO_DATA_FOUND");
  FAIL_IF(SQLFetch(hstmt1) != SQL_NO_DATA_FOUND,"Expected SQL_NO_DATA_FOUND");

  CHECK_STMT_RC(hstmt1, SQLFreeStmt(hstmt1, SQL_CLOSE));

  OK_SIMPLE_STMT(hstmt1, "SELECT 'abcdefghijklmnopqrstuvw'");
  CHECK_STMT_RC(hstmt1, SQLFetch(hstmt1));
  CHECK_STMT_RC(hstmt1, SQLGetData(hstmt1, 1, SQL_WCHAR, wc, 19, &len));
  CHECK_STMT_RC(hstmt1, SQLGetData(hstmt1, 1, SQL_WCHAR, wc, 19, &len));
  CHECK_STMT_RC(hstmt1, SQLGetData(hstmt1, 1, SQL_WCHAR, wc, 19, &len));

  CHECK_DBC_RC(hdbc1, SQLDisconnect(hdbc1));
  CHECK_DBC_RC(hdbc1, SQLFreeHandle(SQL_HANDLE_DBC, hdbc1));

  OK_SIMPLE_STMT(Stmt, "DROP TABLE IF EXISTS t_bug19345");
  
  return OK;
}

ODBC_TEST(charset_gbk)
{
  SQLHANDLE hdbc1;
  SQLHANDLE hstmt1;
  SQLCHAR conn[512], conn_out[512];
  /*
    The fun here is that 0xbf5c is a valid GBK character, and we have 0x27
    as the second byte of an invalid GBK character. mysql_real_escape_string()
    handles this, as long as it knows the character set is GBK.
  */
  SQLCHAR str[]= "\xef\xbb\xbf\x27\xbf\x10";
  SQLSMALLINT conn_out_len;

  sprintf((char *)conn, "DSN=%s;UID=%s;PWD=%s;CHARSET=gbk",
          my_dsn, my_uid, my_pwd);
  
  CHECK_ENV_RC(Env, SQLAllocHandle(SQL_HANDLE_DBC, Env, &hdbc1));

  CHECK_DBC_RC(hdbc1, SQLDriverConnect(hdbc1, NULL, conn, sizeof(conn), conn_out,
                                 sizeof(conn_out), &conn_out_len,
                                 SQL_DRIVER_NOPROMPT));
  CHECK_DBC_RC(hdbc1, SQLAllocStmt(hdbc1, &hstmt1));

  CHECK_STMT_RC(hstmt1, SQLPrepare(hstmt1, (SQLCHAR *)"SELECT ?", SQL_NTS));
  CHECK_STMT_RC(hstmt1, SQLBindParameter(hstmt1, 1, SQL_PARAM_INPUT, SQL_C_CHAR,
                                   SQL_CHAR, 0, 0, str, sizeof(str),
                                   NULL));

  CHECK_STMT_RC(hstmt1, SQLExecute(hstmt1));

  CHECK_STMT_RC(hstmt1, SQLFetch(hstmt1));

  CHECK_STMT_RC(hstmt1, SQLGetData(hstmt1, 1, SQL_CHAR, conn_out, sizeof(conn_out), NULL));
  FAIL_IF(strcmp(conn_out, str) != 0, "comparison failed");
  
  FAIL_IF(SQLFetch(hstmt1) != SQL_NO_DATA, "SQL_NO_DATA expected");

  CHECK_STMT_RC(hstmt1, SQLFreeStmt(hstmt1, SQL_DROP));
  CHECK_DBC_RC(hdbc1, SQLDisconnect(hdbc1));
  CHECK_DBC_RC(hdbc1, SQLFreeHandle(SQL_HANDLE_DBC, hdbc1));

  return OK;
}

ODBC_TEST(t_bug30774)
{
  SQLHDBC hdbc1;
  SQLHSTMT hstmt1;
  SQLCHAR username[65]= {0};

  strcat((char *)username, (char *)my_uid);
  strcat((char *)username, "!!!");

  CHECK_ENV_RC(Env, SQLAllocConnect(Env, &hdbc1));
  CHECK_DBC_RC(hdbc1, SQLConnect(hdbc1, my_dsn, SQL_NTS,
                           username, (SQLSMALLINT)strlen((char *)username) -3,
                           my_pwd, SQL_NTS));
  CHECK_DBC_RC(hdbc1, SQLAllocStmt(hdbc1, &hstmt1));

  OK_SIMPLE_STMT(hstmt1, "SELECT USER()");
  CHECK_STMT_RC(hstmt1, SQLFetch(hstmt1));
  CHECK_STMT_RC(hstmt1, SQLGetData(hstmt1, 1, SQL_CHAR, username, 65, NULL));

  FAIL_IF(strstr(username, "!!!"), "Username changed");
  
  FAIL_IF(SQLFetch(hstmt1) != SQL_NO_DATA_FOUND, "Expected end of data");

  CHECK_STMT_RC(hstmt1, SQLFreeStmt(hstmt1, SQL_DROP));

  CHECK_STMT_RC(hdbc1, SQLDisconnect(hdbc1));
  CHECK_DBC_RC(hdbc1, SQLFreeConnect(hdbc1));

  return OK;
}


#ifdef _WIN32
#  define WE_HAVE_SETUPLIB
#endif

#ifdef WE_HAVE_SETUPLIB
ODBC_TEST(t_bug30840)
{
  HDBC hdbc1;
  SQLCHAR   conn[512], conn_out[1024];
  SQLSMALLINT conn_out_len;

  if (using_dm(Connection))
  {
    diag("test doesn't work with (all) driver manager(s)");
    return SKIP;
  }

  sprintf((char *)conn, "DSN=%s;UID=%s;PWD=%s;NO_PROMPT=1",
          my_dsn, my_uid, my_pwd);

  CHECK_ENV_RC(Env, SQLAllocHandle(SQL_HANDLE_DBC, Env, &hdbc1));

  CHECK_DBC_RC(hdbc1, SQLDriverConnect(hdbc1, (HWND)HWND_DESKTOP, conn, (SQLSMALLINT)strlen(conn),
                                 conn_out, (SQLSMALLINT)sizeof(conn_out), &conn_out_len,
                                 SQL_DRIVER_PROMPT));

  CHECK_DBC_RC(hdbc1, SQLDisconnect(hdbc1));

  CHECK_DBC_RC(hdbc1, SQLFreeHandle(SQL_HANDLE_DBC, hdbc1));

  return OK;
}
#endif


/**
  Bug #30983: SQL Statements limited to 64k
*/
ODBC_TEST(t_bug30983)
{
  SQLCHAR buf[(80 * 1024) + 100]; /* ~80k */
  SQLCHAR *bufp = buf;
  SQLLEN buflen;
  int i, j;

  bufp+= sprintf((char *)bufp, "select '");

  /* fill 1k of each value */
  for (i= 0; i < 80; ++i)
    for (j= 0; j < 512; ++j, bufp += 2)
      sprintf((char *)bufp, "%02x", i);

  sprintf((char *)bufp, "' as val");

  CHECK_STMT_RC(Stmt, SQLExecDirect(Stmt, buf, SQL_NTS));
  CHECK_STMT_RC(Stmt, SQLFetch(Stmt));
  CHECK_STMT_RC(Stmt, SQLGetData(Stmt, 1, SQL_C_CHAR, buf, 0, &buflen));
  is_num(buflen, 80 * 1024);
  return OK;
}


/*
   Test the output string after calling SQLDriverConnect
*/
ODBC_TEST(t_driverconnect_outstring)
{
  HDBC        hdbc1;
  SQLWCHAR    *connw, connw_out[1024];
  SQLSMALLINT conn_out_len;
  SQLCHAR     conna[512], conna_out[1024];

  /* Testing how driver's doing if no out string given. ODBC-17
     ';' at the end is important - otherwise DM adds it while converting connstring for W function */
  sprintf((char*)conna, "DSN=%s;UID=%s;PWD=%s;CHARSET=utf8;", my_dsn, my_uid, my_pwd);
  CHECK_ENV_RC(Env, SQLAllocHandle(SQL_HANDLE_DBC, Env, &hdbc1));

  CHECK_DBC_RC(hdbc1, SQLDriverConnect(hdbc1, NULL, conna, SQL_NTS, NULL,
                                 0, &conn_out_len, SQL_DRIVER_NOPROMPT));
  diag("OutString Length: %d", conn_out_len);
  is_num(conn_out_len, strlen(conna));

  CHECK_DBC_RC(hdbc1, SQLDisconnect(hdbc1));

  CHECK_DBC_RC(hdbc1, SQLDriverConnect(hdbc1, NULL, conna, SQL_NTS, conna_out,
                                 sizeof(conna_out), &conn_out_len, SQL_DRIVER_NOPROMPT));

  is_num(conn_out_len, strlen(conna));
  FAIL_IF(strncmp(conna_out, conna, strlen(conna)), "In and Out connstrings do not match");

  CHECK_DBC_RC(hdbc1, SQLDisconnect(hdbc1));
  
  /* Checking that COMPLETE and COMPLETE_REQUIRED do not fire dialog, if they have enough
     info to establish connection. Also checking that the out connstring in this case is just
     a copy of incoming connstring */
  CHECK_DBC_RC(hdbc1, SQLDriverConnect(hdbc1, NULL, conna, SQL_NTS, NULL,
                                 0, &conn_out_len, SQL_DRIVER_COMPLETE));

  is_num(conn_out_len, strlen(conna));
  IS_STR(conna_out, conna, strlen(conna));

  CHECK_DBC_RC(hdbc1, SQLDisconnect(hdbc1));

  CHECK_DBC_RC(hdbc1, SQLDriverConnect(hdbc1, NULL, conna, SQL_NTS, NULL,
                                 0, &conn_out_len, SQL_DRIVER_COMPLETE_REQUIRED));
 
  is_num(conn_out_len, strlen(conna));
  IS_STR(conna_out, conna, strlen(conna));

  CHECK_DBC_RC(hdbc1, SQLDisconnect(hdbc1));
  CHECK_DBC_RC(hdbc1, SQLFreeHandle(SQL_HANDLE_DBC, hdbc1));

  CHECK_ENV_RC(Env, SQLAllocHandle(SQL_HANDLE_DBC, Env, &hdbc1));

  connw= CW(conna);
  CHECK_DBC_RC(hdbc1, SQLDriverConnectW(hdbc1, NULL, connw, SQL_NTS, connw_out,
                                        sizeof(connw_out), &conn_out_len,
                                        SQL_DRIVER_NOPROMPT));
  is_num(conn_out_len, strlen(conna));
  IS_WSTR(connw_out, connw, strlen(conna));

  CHECK_DBC_RC(hdbc1, SQLDisconnect(hdbc1));

  CHECK_DBC_RC(hdbc1, SQLDriverConnectW(hdbc1, NULL, connw, SQL_NTS, connw_out,
                                        sizeof(connw_out), &conn_out_len,
                                        SQL_DRIVER_COMPLETE));
  is_num(conn_out_len, strlen(conna));
  IS_WSTR(connw_out, connw, strlen(conna));

  CHECK_DBC_RC(hdbc1, SQLDisconnect(hdbc1));

  CHECK_DBC_RC(hdbc1, SQLDriverConnectW(hdbc1, NULL, connw, SQL_NTS, connw_out,
                                        sizeof(connw_out), &conn_out_len,
                                        SQL_DRIVER_COMPLETE_REQUIRED));
  is_num(conn_out_len, strlen(conna));
  IS_WSTR(connw_out, connw, strlen(conna));

  CHECK_DBC_RC(hdbc1, SQLDisconnect(hdbc1));
  CHECK_DBC_RC(hdbc1, SQLFreeHandle(SQL_HANDLE_DBC, hdbc1));
  
  return OK;
}


ODBC_TEST(setnames)
{
  EXPECT_STMT(Stmt, SQLExecDirect(Stmt, "SET NAMES utf8", SQL_NTS), SQL_ERROR);
  EXPECT_STMT(Stmt, SQLExecDirect(Stmt, "SeT NamES utf8", SQL_NTS), SQL_ERROR);
  EXPECT_STMT(Stmt, SQLExecDirect(Stmt, "   set names utf8", SQL_NTS), SQL_ERROR);
  EXPECT_STMT(Stmt, SQLExecDirect(Stmt, "	set names utf8", SQL_NTS), SQL_ERROR);
  EXPECT_STMT(Stmt, SQLExecDirect(Stmt, "/* comment */	set names utf8", SQL_NTS), SQL_ERROR);
  EXPECT_STMT(Stmt, SQLExecDirect(Stmt, "set /* comment */ names utf8", SQL_NTS), SQL_ERROR); 
  return OK;
}


ODBC_TEST(setnames_conn)
{
  HDBC hdbc1;
  SQLCHAR conn[512], conn_out[512];
  SQLSMALLINT conn_out_len;

  sprintf((char *)conn, "DSN=%s;UID=%s;PWD=%s;INITSTMT=set names utf8",
          my_dsn, my_uid, my_pwd);
  
  CHECK_ENV_RC(Env, SQLAllocHandle(SQL_HANDLE_DBC, Env, &hdbc1));

  EXPECT_DBC(hdbc1, SQLDriverConnect(hdbc1, NULL, conn, SQL_NTS, conn_out,
                                     sizeof(conn_out), &conn_out_len,
                                     SQL_DRIVER_NOPROMPT),
             SQL_ERROR);
  SQLDisconnect(hdbc1);

  CHECK_DBC_RC(hdbc1, SQLFreeHandle(SQL_HANDLE_DBC, hdbc1));

  return OK;
}


/**
 Bug #15601: SQLCancel does not work to stop a query on the database server
*/
#ifndef THREAD
ODBC_TEST(sqlcancel)
{
  SQLLEN     pcbLength= SQL_LEN_DATA_AT_EXEC(0);

  CHECK_STMT_RC(Stmt, SQLPrepare(Stmt, "select ?", SQL_NTS));

  CHECK_STMT_RC(Stmt, SQLBindParameter(Stmt, 1,SQL_PARAM_INPUT,SQL_C_CHAR,
                          SQL_VARCHAR,0,0,(SQLPOINTER)1,0,&pcbLength));

  EXPECT_STMT(Stmt, SQLExecute(Stmt), SQL_NEED_DATA);

  /* Without SQLCancel we would get "out of sequence" DM error */
  CHECK_STMT_RC(Stmt, SQLCancel(Stmt));

  CHECK_STMT_RC(Stmt, SQLPrepare(Stmt, "select 1", SQL_NTS));

  CHECK_STMT_RC(Stmt, SQLExecute(Stmt));

  return OK;
}
#else

#ifdef _WIN32
DWORD WINAPI cancel_in_one_second(LPVOID arg)
{
  HSTMT Stmt= (HSTMT)arg;

  Sleep(1000);

  if (SQLCancel(Stmt) != SQL_SUCCESS)
    diag("SQLCancel failed!");

  return 0;
}


ODBC_TEST(sqlcancel)
{
  HANDLE thread;
  DWORD waitrc;

  thread= CreateThread(NULL, 0, cancel_in_one_second, Stmt, 0, NULL);

  /* SLEEP(n) returns 1 when it is killed. */
  OK_SIMPLE_STMT(Stmt, "SELECT SLEEP(5)");
  CHECK_STMT_RC(Stmt, SQLFetch(Stmt));
  is_num(my_fetch_int(Stmt, 1), 1);

  waitrc= WaitForSingleObject(thread, 10000);
  IS(!(waitrc == WAIT_TIMEOUT));

  return OK;
}
#else
void *cancel_in_one_second(void *arg)
{
  HSTMT *Stmt= arg;

  sleep(1);

  if (SQLCancel(Stmt) != SQL_SUCCESS)
    diag("SQLCancel failed!");

  return NULL;
}

#include <pthread.h>

ODBC_TEST(sqlcancel)
{
  pthread_t thread;

  pthread_create(&thread, NULL, cancel_in_one_second, Stmt);

  /* SLEEP(n) returns 1 when it is killed. */
  OK_SIMPLE_STMT(Stmt, "SELECT SLEEP(10)");
  CHECK_STMT_RC(Stmt, SQLFetch(Stmt));
  is_num(my_fetch_int(Stmt, 1), 1);

  pthread_join(thread, NULL);

  return OK;
}
#endif  // ifdef _WIN32
#endif  // ifndef THREAD

ODBC_TEST(t_describe_nulti)
{
  SQLHENV     henv1;
  SQLHDBC     hdbc1;
  SQLHSTMT    hstmt1;
  SQLCHAR ColumnName[64];

  my_options= 67108866;

  ODBC_Connect(&henv1, &hdbc1, &hstmt1);


  OK_SIMPLE_STMT(hstmt1, "DROP TABLE IF EXISTS t1");
  OK_SIMPLE_STMT(hstmt1, "CREATE TABLE t1 (columnX VARCHAR(255))");

  OK_SIMPLE_STMT(hstmt1, "INSERT INTO t1 VALUES ('test')");
  CHECK_STMT_RC(hstmt1, SQLPrepare(hstmt1, "SELECT * FROM t1", SQL_NTS));
 

  CHECK_STMT_RC(hstmt1, SQLDescribeCol(hstmt1, 1, ColumnName, 64, NULL, 0, 0, 0, 0));

  ODBC_Disconnect(henv1, hdbc1, hstmt1);

  return OK;
}


/**
Bug #32014: MyODBC / ADO Unable to open record set using dynamic cursor
*/
ODBC_TEST(t_bug32014)
{
  SQLHENV     henv1;
  SQLHDBC     hdbc1;
  SQLHSTMT    hstmt1;
  SQLUINTEGER info;
  SQLULEN     attr= 0;
  long        i=    0;
  SQLSMALLINT value_len;

  long flags[]= { 0,
                  (131072L << 4)   /*FLAG_FORWARD_CURSOR*/,
                  32               /*FLAG_DYNAMIC_CURSOR*/,
                  (131072L << 4) | 32,
                  0 };

  long expectedInfo[]= { SQL_SO_FORWARD_ONLY|SQL_SO_STATIC,
                         SQL_SO_FORWARD_ONLY,
                         SQL_SO_FORWARD_ONLY|SQL_SO_STATIC|SQL_SO_DYNAMIC,
                         SQL_SO_FORWARD_ONLY|SQL_SO_DYNAMIC };

  long expectedCurType[][4]= {
      {SQL_CURSOR_FORWARD_ONLY, SQL_CURSOR_STATIC,        SQL_CURSOR_STATIC,          SQL_CURSOR_STATIC},
      {SQL_CURSOR_FORWARD_ONLY, SQL_CURSOR_FORWARD_ONLY,  SQL_CURSOR_FORWARD_ONLY,    SQL_CURSOR_FORWARD_ONLY},
      {SQL_CURSOR_FORWARD_ONLY, SQL_CURSOR_STATIC,        SQL_CURSOR_DYNAMIC,         SQL_CURSOR_STATIC},
      {SQL_CURSOR_FORWARD_ONLY, SQL_CURSOR_FORWARD_ONLY,  SQL_CURSOR_FORWARD_ONLY,    SQL_CURSOR_FORWARD_ONLY}};

  do
  {
    my_options= flags[i] +  67108866;
    ODBC_Connect(&henv1, &hdbc1, &hstmt1);

    diag("checking %d (%d)", i, flags[i]);

    /*Checking that correct info is returned*/

    CHECK_STMT_RC(hstmt1, SQLGetInfo(hdbc1, SQL_SCROLL_OPTIONS,
            (SQLPOINTER) &info, sizeof(long), &value_len));
    is_num(info, expectedInfo[i]);

    /*Checking that correct cursor type is set*/
    CHECK_STMT_RC(hstmt1, SQLSetStmtAttr(hstmt1, SQL_ATTR_CURSOR_TYPE
            , (SQLPOINTER)SQL_CURSOR_FORWARD_ONLY, 0));
    CHECK_STMT_RC(hstmt1, SQLGetStmtAttr(hstmt1, SQL_ATTR_CURSOR_TYPE,
            (SQLPOINTER) &attr, 0, NULL));
    is_num(attr, expectedCurType[i][SQL_CURSOR_FORWARD_ONLY]);

    CHECK_STMT_RC(hstmt1, SQLSetStmtAttr(hstmt1, SQL_ATTR_CURSOR_TYPE,
            (SQLPOINTER)SQL_CURSOR_KEYSET_DRIVEN, 0));
    CHECK_STMT_RC(hstmt1, SQLGetStmtAttr(hstmt1, SQL_ATTR_CURSOR_TYPE,
            (SQLPOINTER) &attr, 0, NULL));
    is_num(attr, expectedCurType[i][SQL_CURSOR_KEYSET_DRIVEN]);

    CHECK_STMT_RC(hstmt1, SQLSetStmtAttr(hstmt1, SQL_ATTR_CURSOR_TYPE,
            (SQLPOINTER)SQL_CURSOR_DYNAMIC, 0));
    CHECK_STMT_RC(hstmt1, SQLGetStmtAttr(hstmt1, SQL_ATTR_CURSOR_TYPE,
            (SQLPOINTER) &attr, 0, NULL));
    is_num(attr, expectedCurType[i][SQL_CURSOR_DYNAMIC]);

    /* SQLSet/GetOption are deprecated in favour of SQLSet/GetAttr
       Leaving one just to make sure we don't have problem with old apps,
       but disabling possible warning */
#pragma warning(disable: 4996)
#pragma warning(push)
    CHECK_STMT_RC(hstmt1, SQLSetStmtOption(hstmt1, SQL_CURSOR_TYPE,
            SQL_CURSOR_STATIC ));
    CHECK_STMT_RC(hstmt1, SQLGetStmtOption(hstmt1, SQL_CURSOR_TYPE,
            (SQLPOINTER) &attr));
#pragma warning(pop)
    is_num(attr, expectedCurType[i][SQL_CURSOR_STATIC]);

    ODBC_Disconnect(henv1, hdbc1, hstmt1);

  } while (flags[++i]);

  //SET_DSN_OPTION(0);

  return OK;
}


/*
  Bug #10128 Error in evaluating simple mathematical expression
  ADO calls SQLNativeSql with a NULL pointer for the result length,
  but passes a non-NULL result buffer.
*/
ODBC_TEST(t_bug10128)
{
  SQLCHAR *query= (SQLCHAR *) "select 1,2,3,4";
  SQLCHAR nativesql[1000];
  SQLINTEGER nativelen;
  SQLINTEGER querylen= (SQLINTEGER) strlen((char *)query);

  CHECK_DBC_RC(Connection, SQLNativeSql(Connection, query, SQL_NTS, NULL, 0, &nativelen));
  is_num(nativelen, querylen);

  CHECK_DBC_RC(Connection, SQLNativeSql(Connection, query, SQL_NTS, nativesql, 1000, NULL));
  diag("%s", nativesql);
  IS_STR(nativesql, query, querylen + 1);

  return OK;
}


/**
 Bug #32727: Unable to abort distributed transactions enlisted in MSDTC
*/
ODBC_TEST(t_bug32727)
{
  is_num(SQLSetConnectAttr(Connection, SQL_ATTR_ENLIST_IN_DTC,
                       (SQLPOINTER)1, SQL_IS_UINTEGER), SQL_ERROR);
  return OK;
}


/*
  Bug #28820: Varchar Field length is reported as larger than actual
*/
ODBC_TEST(t_bug28820)
{
  SQLULEN length;
  SQLCHAR dummy[20];
  SQLSMALLINT i;

  OK_SIMPLE_STMT(Stmt, "drop table if exists t_bug28820");
  OK_SIMPLE_STMT(Stmt, "create table t_bug28820 ("
                "x varchar(90) character set latin1,"
                "y varchar(90) character set big5,"
                "z varchar(90) character set utf8)");

  OK_SIMPLE_STMT(Stmt, "select x,y,z from t_bug28820");

  for (i= 0; i < 3; ++i)
  {
    length= 0;
    CHECK_STMT_RC(Stmt, SQLDescribeCol(Stmt, i+1, dummy, sizeof(dummy), NULL,
                                  NULL, &length, NULL, NULL));
	diag("length: %d", length);
    is_num(length, 90);
  }

  OK_SIMPLE_STMT(Stmt, "drop table if exists t_bug28820");
  return OK;
}

ODBC_TEST(t_count)
{
  SQLULEN length;
  SQLCHAR dummy[20];

  OK_SIMPLE_STMT(Stmt, "drop table if exists t_count");
  OK_SIMPLE_STMT(Stmt, "create table t_count (a int)");
  OK_SIMPLE_STMT(Stmt, "INSERT INTO t_count VALUES (1),(2),(3)");
                
  OK_SIMPLE_STMT(Stmt, "select count(*) regcount from t_count");

  length= 0;
  CHECK_STMT_RC(Stmt, SQLDescribeCol(Stmt, 1, dummy, sizeof(dummy), NULL,
                                     NULL, &length, NULL, NULL));
  
  OK_SIMPLE_STMT(Stmt, "drop table if exists t_count");
  return OK;
}

/*
  Bug #31959 - Allows dirty reading with SQL_TXN_READ_COMMITTED
               isolation through ODBC
*/
ODBC_TEST(t_bug31959)
{
  SQLCHAR level[50] = "uninitialized";
  SQLINTEGER i;
  SQLLEN levelid[] = {SQL_TXN_SERIALIZABLE, SQL_TXN_REPEATABLE_READ,
                      SQL_TXN_READ_COMMITTED, SQL_TXN_READ_UNCOMMITTED};
  SQLCHAR *levelname[] = {(SQLCHAR *)"SERIALIZABLE",
                          (SQLCHAR *)"REPEATABLE-READ",
                          (SQLCHAR *)"READ-COMMITTED",
                          (SQLCHAR *)"READ-UNCOMMITTED"};

  CHECK_STMT_RC(Stmt, SQLPrepare(Stmt,
                            (SQLCHAR *)"select @@tx_isolation", SQL_NTS));

  /* check all 4 valid isolation levels */
  for(i = 3; i >= 0; --i)
  {
    CHECK_DBC_RC(Connection, SQLSetConnectAttr(Connection, SQL_ATTR_TXN_ISOLATION,
                                   (SQLPOINTER)levelid[i], 0));
    CHECK_STMT_RC(Stmt, SQLExecute(Stmt));
    CHECK_STMT_RC(Stmt, SQLFetch(Stmt));
    CHECK_STMT_RC(Stmt, SQLGetData(Stmt, 1, SQL_C_CHAR, level, 50, NULL));
    IS_STR(level, levelname[i], strlen((char *)levelname[i]));
    diag("Level = %s\n", level);
    CHECK_STMT_RC(Stmt, SQLFreeStmt(Stmt, SQL_CLOSE));
  }

  /* check invalid value (and corresponding SQL state) */
  is_num(SQLSetConnectAttr(Connection, SQL_ATTR_TXN_ISOLATION, (SQLPOINTER)999, 0),
     SQL_ERROR);
  {
  SQLCHAR     sql_state[6];
  SQLINTEGER  err_code= 0;
  SQLCHAR     err_msg[SQL_MAX_MESSAGE_LENGTH]= {0};
  SQLSMALLINT err_len= 0;

  memset(err_msg, 'C', SQL_MAX_MESSAGE_LENGTH);
  SQLGetDiagRec(SQL_HANDLE_DBC, Connection, 1, sql_state, &err_code, err_msg,
                SQL_MAX_MESSAGE_LENGTH - 1, &err_len);

  IS_STR(sql_state, (SQLCHAR *)"HY024", 5);
  }

  return OK;
}


/*
  Bug #41256 - NULL parameters don't work correctly with ADO.
  The null indicator pointer can be set separately through the
  descriptor field. This wasn't being checked separately.
*/
ODBC_TEST(t_bug41256)
{
  SQLHANDLE apd;
  SQLINTEGER val= 40;
  SQLLEN vallen= 19283;
  SQLLEN ind= SQL_NULL_DATA;
  SQLLEN reslen= 40;
  CHECK_STMT_RC(Stmt, SQLGetStmtAttr(Stmt, SQL_ATTR_APP_PARAM_DESC,
                                &apd, SQL_IS_POINTER, NULL));
  CHECK_STMT_RC(Stmt, SQLBindParameter(Stmt, 1, SQL_PARAM_INPUT, SQL_INTEGER,
                                  SQL_C_LONG, 0, 0, &val, 0, &vallen));
  CHECK_DESC_RC(apd, SQLSetDescField(apd, 1, SQL_DESC_INDICATOR_PTR,
                               &ind, SQL_IS_POINTER));
  OK_SIMPLE_STMT(Stmt, "select ?");
  val= 80;
  CHECK_STMT_RC(Stmt, SQLFetch(Stmt));
  CHECK_STMT_RC(Stmt, SQLGetData(Stmt, 1, SQL_C_LONG, &val, 0, &reslen));
  is_num(SQL_NULL_DATA, reslen);
  is_num(80, val);
  CHECK_STMT_RC(Stmt, SQLFreeStmt(Stmt, SQL_CLOSE));
  return OK;
}


ODBC_TEST(t_bug44971)
{
/*  OK_SIMPLE_STMT(Stmt, "drop database if exists bug44971");
  OK_SIMPLE_STMT(Stmt, "create database bug44971");
  CHECK_DBC_RC(Connection, SQLSetConnectAttr(Connection, SQL_ATTR_CURRENT_CATALOG, "bug44971xxx", 8));
  OK_SIMPLE_STMT(Stmt, "drop database if exists bug44971");*/
  return OK;
}


ODBC_TEST(t_bug48603)
{
  SQLINTEGER timeout, interactive, diff= 1000;
  SQLSMALLINT conn_out_len;
  HDBC hdbc1;
  HSTMT hstmt1;
  SQLCHAR conn[512], conn_out[512], query[53];

  OK_SIMPLE_STMT(Stmt, "select @@wait_timeout, @@interactive_timeout");
  CHECK_STMT_RC(Stmt,SQLFetch(Stmt));

  timeout=      my_fetch_int(Stmt, 1);
  interactive=  my_fetch_int(Stmt, 2);

  CHECK_STMT_RC(Stmt, SQLFreeStmt(Stmt, SQL_CLOSE));

  if (timeout == interactive)
  {
    diag("Changing interactive timeout globally as it is equal to wait_timeout");
    /* Changing globally interactive timeout to be able to test
       if INTERACTIVE option works */
    sprintf((char *)query, "set GLOBAL interactive_timeout=%d", timeout + diff);

    if (!SQL_SUCCEEDED(SQLExecDirect(Stmt, query, SQL_NTS)))
    {
      diag("Don't have rights to change interactive timeout globally - so can't really test if option INTERACTIVE works");
      // Let the testcase does not fail
      diff= 0;
      //return FAIL;
    }

    CHECK_STMT_RC(Stmt, SQLFreeStmt(Stmt, SQL_CLOSE));
  }
  else
  {
    diag("Interactive: %d, wait: %d", interactive, timeout);
    diff= interactive - timeout;
  }

  /* INITSTMT={set @@wait_timeout=%d} */
  sprintf((char *)conn, "DSN=%s;UID=%s;PWD=%s;CHARSET=utf8;INITSTMT=set @@interactive_timeout=%d;INTERACTIVE=1",
    my_dsn, my_uid, my_pwd, timeout+diff);

  if (my_port)
  {
    char pbuff[20];
    sprintf(pbuff, ";PORT=%d", my_port);
    strcat((char *)conn, pbuff);
  }

  CHECK_ENV_RC(Env, SQLAllocHandle(SQL_HANDLE_DBC, Env, &hdbc1));

  CHECK_DBC_RC(hdbc1, SQLDriverConnect(hdbc1, NULL, conn, sizeof(conn), conn_out,
    sizeof(conn_out), &conn_out_len,
    SQL_DRIVER_NOPROMPT));

  CHECK_DBC_RC(hdbc1, SQLAllocStmt(hdbc1, &hstmt1));
  OK_SIMPLE_STMT(hstmt1, "select @@interactive_timeout");
  CHECK_STMT_RC(hstmt1,SQLFetch(hstmt1));

  {
    SQLINTEGER cur_timeout= my_fetch_int(hstmt1, 1);

    CHECK_STMT_RC(hstmt1, SQLFreeStmt(hstmt1, SQL_DROP));
    CHECK_DBC_RC(hdbc1, SQLDisconnect(hdbc1));
    CHECK_DBC_RC(hdbc1, SQLFreeHandle(SQL_HANDLE_DBC, hdbc1));

    if (timeout == interactive)
    {
      /* setting global interactive timeout back if we changed it */
      sprintf((char *)query, "set GLOBAL interactive_timeout=%d", timeout);
      CHECK_STMT_RC(Stmt, SQLExecDirect(Stmt, query, SQL_NTS));
    }

    is_num(timeout + diff, cur_timeout);
  }

  return OK;
}


/*
  Bug#45378 - spaces in connection string aren't removed
*/
ODBC_TEST(t_bug45378)
{
  HDBC hdbc1;
  SQLCHAR conn[512], conn_out[512];
  SQLSMALLINT conn_out_len;

  sprintf((char *)conn, "DSN=%s; UID = {%s} ;PWD= %s ",
          my_dsn, my_uid, my_pwd);
  
  CHECK_ENV_RC(Env, SQLAllocHandle(SQL_HANDLE_DBC, Env, &hdbc1));

  CHECK_DBC_RC(hdbc1, SQLDriverConnect(hdbc1, NULL, conn, SQL_NTS, conn_out,
                                 sizeof(conn_out), &conn_out_len,
                                 SQL_DRIVER_NOPROMPT));
  CHECK_DBC_RC(hdbc1, SQLDisconnect(hdbc1));
  CHECK_DBC_RC(hdbc1, SQLFreeHandle(SQL_HANDLE_DBC, hdbc1));

  return OK;
}


ODBC_TEST(t_mysqld_stmt_reset)
{
  OK_SIMPLE_STMT(Stmt, "drop table if exists t_reset");
  OK_SIMPLE_STMT(Stmt, "create table t_reset (a int)");
  OK_SIMPLE_STMT(Stmt, "INSERT INTO t_reset(a) VALUES (1),(2),(3)");

  /* Succesful query deploying PS */
  OK_SIMPLE_STMT(Stmt, "SELECT count(*) FROM t_reset");
  CHECK_STMT_RC(Stmt,SQLFetch(Stmt));
  is_num(my_fetch_int(Stmt, 1), 3);
  CHECK_STMT_RC(Stmt, SQLFreeStmt(Stmt, SQL_CLOSE));

  /* Unsuccessful quer */
  EXPECT_STMT(Stmt, SQLExecDirect(Stmt, "SELECT count(*) FROM t_reset_nonexistent", SQL_NTS), SQL_ERROR);
  CHECK_STMT_RC(Stmt, SQLFreeStmt(Stmt, SQL_CLOSE));

  /* Successful directly executed query */
  OK_SIMPLE_STMT(Stmt, "delete from t_reset where a=2");
  CHECK_STMT_RC(Stmt, SQLFreeStmt(Stmt, SQL_CLOSE));

  /* And now successful query again */
  OK_SIMPLE_STMT(Stmt, "SELECT count(*) FROM t_reset");
  CHECK_STMT_RC(Stmt,SQLFetch(Stmt));
  is_num(my_fetch_int(Stmt, 1), 2);

  CHECK_STMT_RC(Stmt, SQLFreeStmt(Stmt, SQL_CLOSE));

  OK_SIMPLE_STMT(Stmt, "drop table if exists t_reset");
  return OK;
}


ODBC_TEST(t_odbc32)
{
  HDBC        hdbc1;
  SQLCHAR     conn[512];
  SQLUINTEGER packet_size= 0;

  sprintf((char *)conn, "DSN=%s;", my_dsn);
  
  CHECK_ENV_RC(Env, SQLAllocHandle(SQL_HANDLE_DBC, Env, &hdbc1));
  CHECK_DBC_RC(hdbc1, SQLSetConnectAttr(hdbc1, SQL_ATTR_PACKET_SIZE, (SQLPOINTER)(4096*1024), 0));

  CHECK_DBC_RC(hdbc1, SQLDriverConnect(hdbc1, NULL, conn, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT));

  CHECK_DBC_RC(hdbc1, SQLGetConnectAttr(hdbc1, SQL_ATTR_PACKET_SIZE, (SQLPOINTER)&packet_size, 0, NULL));

  diag("Packet size is %u", packet_size);

  CHECK_DBC_RC(hdbc1, SQLDisconnect(hdbc1));
  CHECK_DBC_RC(hdbc1, SQLFreeHandle(SQL_HANDLE_DBC, hdbc1));

  return OK;
}


ODBC_TEST(t_gh_issue3)
{
  CHECK_STMT_RC(Stmt, SQLExecDirect(Stmt, "\nSELECT 1", SQL_NTS));
  CHECK_STMT_RC(Stmt, SQLFetch(Stmt));
  is_num(my_fetch_int(Stmt, 1), 1);
  CHECK_STMT_RC(Stmt, SQLFreeStmt(Stmt, SQL_CLOSE));

  CHECK_STMT_RC(Stmt, SQLExecDirect(Stmt, "\tSELECT 2", SQL_NTS));
  CHECK_STMT_RC(Stmt, SQLFetch(Stmt));
  is_num(my_fetch_int(Stmt, 1), 2);
  CHECK_STMT_RC(Stmt, SQLFreeStmt(Stmt, SQL_CLOSE));

  CHECK_STMT_RC(Stmt, SQLExecDirect(Stmt, "\t SELECT 3", SQL_NTS));
  CHECK_STMT_RC(Stmt, SQLFetch(Stmt));
  is_num(my_fetch_int(Stmt, 1), 3);
  CHECK_STMT_RC(Stmt, SQLFreeStmt(Stmt, SQL_CLOSE));

  CHECK_STMT_RC(Stmt, SQLExecDirect(Stmt, "\n\t\n  \t\n  \n \t\t\t\t SELECT 4", SQL_NTS));
  CHECK_STMT_RC(Stmt, SQLFetch(Stmt));
  is_num(my_fetch_int(Stmt, 1), 4);
  CHECK_STMT_RC(Stmt, SQLFreeStmt(Stmt, SQL_CLOSE));

  return OK;
}


ODBC_TEST(t_odbc48)
{
  OK_SIMPLE_STMT(Stmt, "DROP PROCEDURE IF EXISTS test_odbc_48");
  OK_SIMPLE_STMT(Stmt,
    "CREATE PROCEDURE test_odbc_48()"
    "BEGIN"
    " SELECT 1 AS ret;"
    "END");
  OK_SIMPLE_STMT(Stmt, "{ CALL test_odbc_48() }");
  CHECK_STMT_RC(Stmt, SQLFreeStmt(Stmt, SQL_CLOSE));
  OK_SIMPLE_STMT(Stmt, "DROP PROCEDURE test_odbc_48");

  return OK;
}

/* Verifying that charset names are case insensitive */
ODBC_TEST(t_odbc69)
{
  HDBC hdbc1;
  SQLCHAR   conn[512], conn_out[1024];
  SQLSMALLINT conn_out_len;

  /* Testing also that key names are case insensitve. Supposing, that there is no mariadb/mysql on 3310 port with same login credentials */
  sprintf((char *)conn, "DSN=%s;UID=%s;PWD=%s;PORT=3310;DATABASE=%s;OPTION=%lu;SERVER=%s;PoRt=%u;charset=UTF8",
    my_dsn, my_uid, my_pwd, my_schema, my_options, my_servername, my_port);

  CHECK_ENV_RC(Env, SQLAllocHandle(SQL_HANDLE_DBC, Env, &hdbc1));

  /* If everything is right, right port value will override the wrong one, and connect will be successful */
  CHECK_DBC_RC(hdbc1, SQLDriverConnect(hdbc1, NULL, conn, (SQLSMALLINT)strlen(conn),
    conn_out, (SQLSMALLINT)sizeof(conn_out), &conn_out_len,
    SQL_DRIVER_NOPROMPT));

  CHECK_DBC_RC(hdbc1, SQLDisconnect(hdbc1));

  CHECK_DBC_RC(hdbc1, SQLFreeHandle(SQL_HANDLE_DBC, hdbc1));

  return OK;
}

/* If connection handle re-used, it would try to select database used in previous connection */
ODBC_TEST(t_odbc91)
{
  HDBC      hdbc;
  HSTMT     hstmt;
  SQLCHAR   conn[512], conn_out[1024], buffer[32];
  SQLSMALLINT conn_out_len;

  OK_SIMPLE_STMT(Stmt, "DROP DATABASE IF EXISTS t_odbc91");
  OK_SIMPLE_STMT(Stmt, "CREATE DATABASE t_odbc91");

  /* Connecting to newly created tatabase */
  sprintf((char *)conn, "DSN=%s;UID=%s;PWD=%s;DATABASE=%s;OPTION=%lu;SERVER=%s%s",
    my_dsn, my_uid, my_pwd, "t_odbc91", my_options, my_servername, ma_strport);

  CHECK_ENV_RC(Env, SQLAllocHandle(SQL_HANDLE_DBC, Env, &hdbc));

  CHECK_DBC_RC(hdbc, SQLDriverConnect(hdbc, NULL, conn, (SQLSMALLINT)strlen(conn),
    conn_out, (SQLSMALLINT)sizeof(conn_out), &conn_out_len,
    SQL_DRIVER_NOPROMPT));

  CHECK_DBC_RC(hdbc, SQLDisconnect(hdbc));

  OK_SIMPLE_STMT(Stmt, "DROP DATABASE t_odbc91");

  /* Now we do not specify any database */
  sprintf((char *)conn, "DSN=%s;UID=%s;PWD=%s;OPTION=%lu;SERVER=%s;DATABASE=%s%s",
    my_dsn, my_uid, my_pwd, my_options, my_servername, my_schema, ma_strport);

  CHECK_DBC_RC(hdbc, SQLDriverConnect(hdbc, NULL, conn, (SQLSMALLINT)strlen(conn),
    conn_out, (SQLSMALLINT)sizeof(conn_out), &conn_out_len,
    SQL_DRIVER_NOPROMPT));

  CHECK_DBC_RC(hdbc, SQLDisconnect(hdbc));

  /* Now testing scenario, there default database is set via connetion attribute, and connection handler is re-used
     after disconnect. This doesn't work with UnixODBC, because smart UnixODBC implicicitly deallocates connection handle
     when SQLDisconnect is called */
  if (UnixOdbc(Env))
  {
    diag("UnixODBC detected - Skipping part of the test");
    return OK;
  }
  OK_SIMPLE_STMT(Stmt, "CREATE DATABASE t_odbc91");
  CHECK_DBC_RC(hdbc, SQLSetConnectAttr(hdbc, SQL_ATTR_CURRENT_CATALOG, (SQLPOINTER)"t_odbc91", SQL_NTS));

  sprintf((char *)conn, "Driver=%s;UID=%s;PWD=%s;OPTION=%lu;SERVER=%s%s",
    my_drivername, my_uid, my_pwd, my_options, my_servername, ma_strport);

  CHECK_DBC_RC(hdbc, SQLDriverConnect(hdbc, NULL, conn, (SQLSMALLINT)strlen(conn),
    conn_out, (SQLSMALLINT)sizeof(conn_out), &conn_out_len,
    SQL_DRIVER_NOPROMPT));

  CHECK_DBC_RC(hdbc, SQLGetConnectAttr(hdbc, SQL_ATTR_CURRENT_CATALOG, (SQLPOINTER)buffer, sizeof(buffer), NULL));

  IS_STR(buffer, "t_odbc91", sizeof("t_odbc91"));
  buffer[0]= '\0';

  CHECK_DBC_RC(hdbc, SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt));

  OK_SIMPLE_STMT(hstmt, "SELECT DATABASE()");
  CHECK_STMT_RC(hstmt, SQLFetch(hstmt));
  IS_STR(my_fetch_str(hstmt, buffer, 1), "t_odbc91", sizeof("t_odbc91"));

  CHECK_DBC_RC(hdbc, SQLDisconnect(hdbc));

  buffer[0]= '\0';

  /* Checking that attribute value is preserved - this doesn't work with UnixODBC */
  CHECK_DBC_RC(hdbc, SQLGetConnectAttr(hdbc, SQL_ATTR_CURRENT_CATALOG, (SQLPOINTER)buffer, sizeof(buffer), NULL));
  IS_STR(buffer, "t_odbc91", sizeof("t_odbc91"));

  CHECK_DBC_RC(hdbc, SQLDriverConnect(hdbc, NULL, conn, (SQLSMALLINT)strlen(conn),
    conn_out, (SQLSMALLINT)sizeof(conn_out), &conn_out_len,
    SQL_DRIVER_NOPROMPT));
  CHECK_DBC_RC(hdbc, SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt));

  OK_SIMPLE_STMT(hstmt, "SELECT DATABASE()");
  CHECK_STMT_RC(hstmt, SQLFetch(hstmt));
  IS_STR(my_fetch_str(hstmt, buffer, 1), "t_odbc91", sizeof("t_odbc91"));
 
  CHECK_STMT_RC(hstmt, SQLFreeStmt(hstmt, SQL_CLOSE));

  CHECK_DBC_RC(hdbc, SQLDisconnect(hdbc));
  CHECK_DBC_RC(hdbc, SQLFreeHandle(SQL_HANDLE_DBC, hdbc));
  OK_SIMPLE_STMT(Stmt, "DROP DATABASE t_odbc91");

  return OK;
}


ODBC_TEST(t_odbc137)
{
  SQLHDBC    Hdbc;
  SQLHSTMT   Hstmt;
  char       buffer[256], AllAnsiChars[258], AllAnsiHex[512];
  const char Charset[][16]= {"latin1", "cp850", "cp1251", "cp866", "cp852", "cp1250", "latin2", "latin5" ,"latin7",
                             "cp1256", "cp1257", "geostd8", "greek", "koi8u", "koi8r", "hebrew", "macce", "macroman",
                             "dec8", "hp8", "armscii8", "ascii", "swe7", "tis620", "keybcs2"};
                        /* ������ in latin1(cp= 1252),  ������ in cp850(cp= 437_,  () in win1251 */
  /*const char TestStr[][8]={ "\xe5\xe4\xf6\xc5\xc4\xd6", "\x86\x84\x94\x8f\x8e\x99", "\xb4\xa2\xfa\xa5\xa1\xda" },
    HexStr[][16]={ "0xE5E4F6C5C4D6", "0x8684948F8E99", "0xB4A2FAA5A1DA" };*/
  const char CreateStmtTpl[]= "CREATE TABLE `t_odbc137` (\
                          `val` TEXT DEFAULT NULL\
                          ) ENGINE=InnoDB DEFAULT CHARSET=%s";
  char       CreateStmt[sizeof(CreateStmtTpl) + sizeof(Charset[0])];
  const char InsertStmtTpl[]= "INSERT INTO t_odbc137(val)  VALUES('%s')";
  char       InsertStmt[sizeof(InsertStmtTpl) + sizeof(AllAnsiChars)];// TestStr[0])];
  const char SelectStmtTpl[]= "SELECT * FROM t_odbc137 WHERE val = 0x%s";
  char       SelectStmt[sizeof(SelectStmtTpl) + sizeof(AllAnsiHex)];// HexStr[0])];
  unsigned int i, j, Escapes= 0;

  OK_SIMPLE_STMT(Stmt, "DROP TABLE IF EXISTS `t_odbc137`");
  CHECK_ENV_RC(Env, SQLAllocConnect(Env, &Hdbc));

  for (i= 1; i < 256; ++i)
  {
    if (i == '\'' || i == '\\')
    {
      AllAnsiChars[i-1 + Escapes]= '\\';
      ++Escapes;
    }

    AllAnsiChars[i-1 + Escapes]= i;
    sprintf(AllAnsiHex + (i-1)*2, "%02x", i);
  }

  AllAnsiChars[255 + Escapes]= AllAnsiHex[511]= '\0';
  
  for (i= 0; i < sizeof(Charset)/sizeof(Charset[0]); ++i)
  {
    diag("Charset: %s", Charset[i]);
    
    Hstmt= ConnectWithCharset(&Hdbc, Charset[i], NULL);
    FAIL_IF(Hstmt == NULL, "");

    sprintf(CreateStmt, CreateStmtTpl, Charset[i]);
    OK_SIMPLE_STMT(Hstmt, CreateStmt);

    sprintf(InsertStmt, InsertStmtTpl, AllAnsiChars);// TestStr[i]);
    OK_SIMPLE_STMT(Hstmt, InsertStmt);

    sprintf(SelectStmt, SelectStmtTpl, AllAnsiHex);// HexStr[i]);
    OK_SIMPLE_STMT(Hstmt, SelectStmt);

    FAIL_IF(SQLFetch(Hstmt) == SQL_NO_DATA, "Wrong data has been stored in the table");
    /* We still need to make sure that the string has been delivered correctly */
    my_fetch_str(Hstmt, buffer, 1);
    /* AllAnsiChars is escaped, so we cannot compare result string against it */
    for (j= 1; j < 256; ++j)
    {
      is_num((unsigned char)buffer[j - 1], j);
    }
    CHECK_STMT_RC(Hstmt, SQLFreeStmt(Hstmt, SQL_DROP));
    OK_SIMPLE_STMT(Stmt, "DROP TABLE `t_odbc137`");
    CHECK_DBC_RC(Hdbc, SQLDisconnect(Hdbc));
  }

  CHECK_DBC_RC(Hdbc, SQLFreeConnect(Hdbc));

  return OK;
}

#ifdef _WIN32
DWORD WINAPI FireQueryInThread(LPVOID arg)
{
  HSTMT hStmt= (HSTMT)arg;

  SQLExecDirect(hStmt, "SELECT 1", SQL_NTS);

  return 0;
}


ODBC_TEST(t_odbc139)
{
  SQLHDBC  Hdbc;
  SQLHSTMT Hstmt;
  unsigned long Compression= 2048;
  HANDLE Thread;
  DWORD WaitRc;

  if (ServerNotOlderThan(Connection, 10, 2, 0))
  {
    skip("Waiting for the fix in Connector/C for servers > 10.2.0");
  }
  CHECK_ENV_RC(Env, SQLAllocConnect(Env, &Hdbc));

  CHECK_DBC_RC(Hdbc, SQLSetConnectAttr(Hdbc, SQL_ATTR_CURRENT_CATALOG, (SQLPOINTER)"test", 4));
  Hstmt= DoConnect(Hdbc, NULL, NULL, NULL, 0, NULL, &Compression, NULL, NULL);


  Thread= CreateThread(NULL, 0, FireQueryInThread, Hstmt, 0, NULL);
  WaitRc= WaitForSingleObject(Thread, 1500);
  FAIL_IF(WaitRc == WAIT_TIMEOUT, "Direct Execution has taken too long time");

  CHECK_STMT_RC(Hstmt, SQLFetch(Hstmt));

  CHECK_STMT_RC(Hstmt, SQLFreeStmt(Hstmt, SQL_DROP));
  CHECK_DBC_RC(Hdbc, SQLDisconnect(Hdbc));
  CHECK_DBC_RC(Hdbc, SQLFreeConnect(Hdbc));

  return OK;
}
#endif

ODBC_TEST(t_odbc162)
{
  SQLSMALLINT ColumnCount;
  SQLRETURN rc;

  rc= SQLExecDirect(Stmt, "with x as (select 1 as `val`\
                                   union all\
                                   select 2 as `val`\
                                   union all\
                                   select 3 as `val`)\
                        select repeat(cast(x.val as nchar), x.val * 2) as `string`,\
                               repeat(cast(x.val as char), x.val * 2) as `c_string`,\
                               cast(repeat(char(x.val), x.val * 12000) as binary) as `binary`,\
                               x.val as `index`\
                        from x", SQL_NTS);

  if (SQL_SUCCEEDED(rc))
  {
    CHECK_STMT_RC(Stmt, SQLNumResultCols(Stmt, &ColumnCount));

    is_num(ColumnCount, 4);
    CHECK_STMT_RC(Stmt, SQLFreeStmt(Stmt, SQL_CLOSE));
  }
  else
  {
    /* Old server does not support it */
    CHECK_SQLSTATE(Stmt, "42000");
  }

  return OK;
}


ODBC_TEST(t_replace)
{
  if (!ServerNotOlderThan(Connection, 10, 2, 0))
  {
    skip("REPLACE SQL command is not supported by your server version");
  }
  
  OK_SIMPLE_STMT(Stmt, "DROP TABLE IF EXISTS `t_odbc_replace`");
  OK_SIMPLE_STMT(Stmt, "CREATE TABLE `t_odbc_replace` (`uuid` VARCHAR(64), PRIMARY KEY (`uuid`)) ENGINE=InnoDB");
  OK_SIMPLE_STMT(Stmt, "REPLACE INTO `t_odbc_replace` (`uuid`) VALUES(\"5b7fe80c-7de1-4744-ab33-3f65f26726f6\"),\
(\"3ce73838-72f2-4aed-9f45-b7cf15f1279b\"),\
(\"babf4138-cdde-4c3f-8b28-cc7c8ef7033f\"),\
(\"80abd3ca-37d3-4478-8de3-137f14b0aa69\"),\
(\"4cc6c8c4-dfdc-47ec-923c-2d60b6a34053\"),\
(\"1e802c01-4849-499c-b707-839f327d2a6e\"),\
(\"5b7cc14e-0339-49a9-abee-cb80afa438ee\"),\
(\"ad3be64a-5cfa-4f79-84fb-e41433cca31b\"),\
(\"44ce8320-5100-4a2e-b078-18a855c31398\"),\
(\"95e7985e-c2a1-4f70-840e-23b73d9fa933\"),\
(\"5b08812f-4750-484d-8041-b304536b836f\"),\
(\"890651bb-fadb-445d-8142-339219c73b53\"),\
(\"70967597-5585-447b-a91d-5b59226a374b\"),\
(\"cb6bf816-ff91-47e2-8f84-785c8132e8a5\"),\
(\"3ef7caf9-84e4-4922-a7be-8fa611ef3fba\"),\
(\"0f7faaf6-aff2-4198-81e3-896496afbb2b\"),\
(\"c4d6a9de-f355-4b61-a773-f9b00a9f12c9\"),\
(\"88c02aa5-0c13-4da1-bcf9-a1e05a8260ad\"),\
(\"8d1bbec0-ece1-42a6-be30-d05a2c650e69\"),\
(\"8efd401a-6308-40e3-9e15-0a8f92cbdbea\"),\
(\"e179182d-82ee-4a76-a34f-c1a414213b1c\"),\
(\"748308ce-5d45-448f-9ee0-597811d7e587\"),\
(\"bfaaf614-e815-4706-83b4-044fa60fb80e\"),\
(\"3c8d7733-fca8-40b6-8bce-f68ba8c74c55\"),\
(\"f118c366-fdda-4063-bd4a-da447325c5c5\"),\
(\"af56d425-46d0-4c75-ac02-6f74fa8362e2\"),\
(\"541937b0-bf92-40c3-a430-c92e2b073114\"),\
(\"d192dd73-b2c8-4348-9b58-e01554f76679\"),\
(\"d56b1182-a077-47bd-9954-e6d62a75a05a\"),\
(\"3a87fe27-af69-416d-b87c-63359e60a8f9\"),\
(\"b2d5d12f-18e5-4a9f-87d4-66bf86d89951\"),\
(\"18f949ca-c0e0-4dba-be0f-e78354d04cf5\"),\
(\"f637efda-c264-4a77-80f7-cfd2edc470db\"),\
(\"1377a6ab-9325-4231-bdab-261344fcf2e2\"),\
(\"c00e0d8e-36b8-4782-9793-7b2b56af6b7f\"),\
(\"96fe1f4b-a9d5-4f36-bc01-ac29e822316a\"),\
(\"a3d9fe79-260a-4051-9d40-cd364cb9b350\"),\
(\"a9fea9bd-6371-4058-86e3-fb240af089df\"),\
(\"ce512e8c-d162-4bdb-b1e5-fe59de474f08\"),\
(\"5da05ca2-4c55-4dc0-b57c-6a855b30c919\"),\
(\"49bc86b4-2570-41fe-abdc-8b177b6c931d\"),\
(\"c45030e4-7c11-4a53-80ae-1472f9cb1ba9\"),\
(\"9db937fb-9baa-4ebc-9c19-84335772955f\"),\
(\"889338ce-97ad-4d39-a60d-32ed405f240e\"),\
(\"6289819a-4b3e-43f5-896a-3b4ae65ec9eb\"),\
(\"9383539a-cd0b-4b95-b5fc-95c5ad9b206c\"),\
(\"86b1ffb4-2e62-4868-94a0-4cd369077a56\"),\
(\"e2dfa7fe-f4f9-47bb-89aa-9a5b05345803\"),\
(\"62e68088-67fd-4fa7-a589-eee1a16bf24d\"),\
(\"ee5b0616-14a7-47c2-ace0-f8f125bf5832\"),\
(\"b03d3e73-5974-4c0c-a46d-6279fde8a263\"),\
(\"09bbd84e-e60e-4f31-a44b-453563bd7abe\"),\
(\"2c2728c7-6983-4d88-81b8-f7f2ff52a06a\"),\
(\"57eb943a-3e78-4ec4-8334-402fd607346c\"),\
(\"0cd4acad-dee7-44cd-9803-35219d6602f7\"),\
(\"57c33b05-7c63-4780-93be-46466f2da4fa\"),\
(\"bf349102-865c-485d-b9a6-789372a3ee72\"),\
(\"352c5c22-f466-4342-a9e4-412e410925b2\"),\
(\"e2acdfb1-87a1-4ac1-a60f-8280ccbd948d\"),\
(\"6cd49199-0e61-4042-b44f-b975a3b2ee71\"),\
(\"ac59c9f5-cf2e-4ab0-84fc-23e1f8b9a540\"),\
(\"0e6c7772-2fb4-439f-b129-dc46f3690926\"),\
(\"2771fdfa-2dae-4e94-8d65-e3914db8583a\"),\
(\"7af76524-ee9b-416e-bb4d-055646615574\"),\
(\"7c11457d-d76b-4815-9b7a-6ddac45d014f\"),\
(\"0781fc72-8fbf-4877-8f0f-35f7c11f399d\"),\
(\"68a4b149-6b08-418d-a5bf-bc45b7752463\"),\
(\"3b9b4bba-6f7d-42d3-b18b-701da05e2b28\"),\
(\"ecac75db-872f-4d7e-8234-8eb2758d7e6c\"),\
(\"79fe2e47-0aa5-46a2-b115-c854a28060bb\"),\
(\"227db73c-4ecb-4143-b0b5-974d93959984\"),\
(\"2b43269a-c7b4-4a3a-a0c2-d28c49753465\"),\
(\"0b64be51-a8ad-4e49-9282-552a1ec8e42c\"),\
(\"8fef2818-0cbf-46fc-9719-bfb76b460bf8\"),\
(\"be1e3a70-197d-4f6e-b41f-013c52708c74\"),\
(\"da2b8613-8f7c-48bc-85f9-a32db5417747\"),\
(\"bd3e60cb-23fd-4f54-b083-7433aa6abd69\"),\
(\"ed0375e2-2d3c-4aba-a7b6-e413dec6adc6\"),\
(\"440a0ec9-8bcd-4abd-bc7c-47b2d261bad9\"),\
(\"8bfaf0af-34f1-4ac4-a8e1-bc2585341382\"),\
(\"e617bbea-fc2d-4371-acc7-1a2b2d15e54c\"),\
(\"9356407c-3095-4cf4-9f0d-b9a38b497cd6\"),\
(\"ac6a5102-f4c5-4adb-80c4-fcf5fa540b84\"),\
(\"43202ede-365d-4353-8b83-9d8d82d28b28\"),\
(\"1add03b0-e530-4f78-9114-396ffa05b199\"),\
(\"6df7c8d5-6b09-4df0-b0f4-90714a87085c\"),\
(\"f7ef8542-edcf-4f83-8e8c-1e0d4a2e73f7\"),\
(\"5830a5dc-2989-4b31-b585-d9a2a4b75adc\"),\
(\"b3cefe04-fec3-4ffb-bbce-6cb6ceb73d61\"),\
(\"be34f895-4417-4cd4-9782-fd14b4e31328\"),\
(\"23f0d4a8-6b9f-45de-a5c5-49507c4362fb\"),\
(\"c3825733-4ca9-4f1b-a790-ccfc10bb9ffa\"),\
(\"1f439cfe-f151-42a8-9edf-5c332dcc98ac\"),\
(\"133b63c2-27b2-494c-ac54-42ef8dced01c\"),\
(\"47ab46a6-ed9c-444f-b20a-8c9c647fb11b\"),\
(\"25a0f6f7-3242-40ec-a8b4-963da14ba837\"),\
(\"1ff8d628-2331-4691-8835-bcde5ca12a90\"),\
(\"a2ea2ba1-e12d-4efe-8f97-8eeb122d77b1\"),\
(\"b653a3c2-1239-46ca-bc2c-2fa4adcb61e0\"),\
(\"af63a702-9f1b-462b-822a-252d424ea1b7\"),\
(\"fbc7fbca-7a80-47f7-a483-50d62a96eefb\"),\
(\"b801bd55-ed9a-457e-917c-30805a3f0120\"),\
(\"2c8845c4-b054-4e28-83b7-e132d0d08253\"),\
(\"f1978ed4-be17-4f12-85e0-fd435a49550f\"),\
(\"92629d5a-027c-4b84-8787-a40144318dfa\"),\
(\"e37140b4-4f7e-4b1b-9851-cadb1914c232\"),\
(\"9aaef9ac-5605-4311-9e54-2e677484be96\"),\
(\"165df507-62b8-499a-b6bf-b6c69716e698\"),\
(\"e2e84af9-5a5d-4749-b6d3-6f83ce4b8e69\"),\
(\"2767b49f-5fcb-41d1-b069-be955dd75fcc\"),\
(\"8575f313-2f01-4f58-8805-852eff7d51e7\"),\
(\"bb2f9ec3-cd6c-4bbc-a2a1-7ace39d0e8fb\"),\
(\"904ba035-1464-497c-a457-f66e640d7d41\"),\
(\"7f876416-60c1-4245-9f9a-a6d407d5c6d4\"),\
(\"92d9af49-35e0-4ac4-81c8-88dfd9aed7c6\"),\
(\"f20aef30-815d-4796-b364-eea23d05d948\"),\
(\"6b4d97dc-f481-43ca-b87e-a7b1bda6a680\"),\
(\"823e05bd-7ad4-4ad5-b174-2c1e27f17663\"),\
(\"088cf83e-e84a-484a-b151-8760be1de974\"),\
(\"8bb161d0-1867-44a0-95d0-75acd800f531\"),\
(\"0798763d-eb58-4b7c-950c-9540d34a5a9f\"),\
(\"47c9accd-9085-4a96-bf75-033b5af541c1\"),\
(\"89e07c25-b380-4ccd-ace9-18e09f84acd2\"),\
(\"e114895c-45ff-4ab7-9870-c61c21670f15\"),\
(\"d4e7e3f2-af95-439e-b74a-f0586828bc37\"),\
(\"b60f4dce-a645-4626-9499-da2cd7af73d6\"),\
(\"5b4e6a7f-39f5-454c-8bdd-4f6e41b37a34\"),\
(\"5db6f2aa-2edc-4126-926a-9998da4b38bd\"),\
(\"1a2f8d0f-3a8e-4bd6-921c-ff283f72b7d6\"),\
(\"c80c0ac2-5fe4-4189-aa6b-76ed05070581\"),\
(\"e87c4e2a-6976-4bdd-a7b5-8d1d63ef6c93\"),\
(\"93091c62-7356-4b2f-a277-3ce74d39f0a3\"),\
(\"84ac8533-6975-4834-8cd4-a9df75140d9a\"),\
(\"47047b2a-4e5f-48d3-a81c-908e36aa74d8\"),\
(\"45785654-9978-4b16-9d13-c2c8b12ee620\"),\
(\"ca275e72-b5da-4c43-8a18-cd3225d126b8\"),\
(\"4e33ce01-d28b-479f-b16a-7aa382afee13\"),\
(\"3295b297-618d-4949-a476-d8211305faa5\"),\
(\"18522a4f-5ffb-4994-936a-4974cf0f1fb7\"),\
(\"694d1f76-b823-40ea-8d28-38f3e6aa77db\"),\
(\"eb5b5222-07cc-4f76-b652-cce942c31f4b\"),\
(\"f138e5f1-4165-4b17-b8b5-3ab6d0ba4406\"),\
(\"a45cebdb-7642-4495-ab6c-9b7b5c8cbf58\"),\
(\"33247640-39e4-4570-85e8-06311e7b67c6\"),\
(\"ba6cc554-8d41-4205-9bdb-b6fb670c37a1\"),\
(\"70332181-a298-4c89-b12d-fe5457afa573\"),\
(\"15dd39b4-8e32-4cb5-9bb2-7598b2b68f7a\"),\
(\"bbe2eaae-152b-466e-93f2-5d73a3181426\"),\
(\"d4964e50-cd71-408f-a4fd-b8210f3340e2\"),\
(\"416586de-a1b5-48b8-b061-8702172feb7c\"),\
(\"5e9f84f3-e843-471c-a275-346a5d8af8c9\"),\
(\"e0153c6b-62a6-4271-9e53-b792990dd6ed\"),\
(\"60eaf8bc-f0ee-4a04-8a89-89d137d61b1f\"),\
(\"377b224c-c9a9-4825-8070-a7b8c4c90b3d\"),\
(\"d4d1bbd5-20cc-4b9e-965a-9edef1ad5bbc\"),\
(\"c73c8907-d47a-44df-af52-7cafe8a5c997\"),\
(\"416fbd13-96c7-4690-b5e3-916be454e7b9\"),\
(\"3a973385-7421-40b4-b87b-19dcdc8c3a02\"),\
(\"7c4feb3b-bd99-416d-b729-51f950151a45\"),\
(\"d8427e40-6746-48bd-a49d-766075c1d464\"),\
(\"bbeee4e8-b2f1-4b16-acd7-65f26f90324c\"),\
(\"96688efd-a0f3-421d-91ca-d2d90fff822d\"),\
(\"e93d4c89-ec2b-4f59-81c4-4b584780df73\"),\
(\"457cbcf9-8a53-40ca-9ee0-8faf6feead50\"),\
(\"7059fad8-b1f7-48d2-9025-4c9c2ec08487\"),\
(\"3f5a561f-015d-4213-adfa-e5fc168c7a7d\"),\
(\"0824ebae-d62d-4635-b82c-f29dcc1d95eb\"),\
(\"d76b80ba-8f51-48ea-a8d9-dbbfbbad314b\"),\
(\"2019a85b-aa92-44fd-9c0d-cb4f9007e947\"),\
(\"ab8f24c7-3310-426f-ad61-4d8367eefdb1\"),\
(\"3bc35fc8-827e-4e46-8fa6-69110643baac\"),\
(\"6e4b6c56-224b-4b69-b369-a9879655b1da\"),\
(\"6bb539b0-c07e-4f27-b7cf-6e8f5f2b93e5\"),\
(\"d4dfe64e-0de9-46a1-8414-27312515f63c\"),\
(\"01ac5320-11bb-43fd-91d9-eacd3f463cdf\"),\
(\"671e2dff-a7a4-4e50-998b-0f0ffb418cce\"),\
(\"1d6b29db-647f-4107-845f-73af7eb57d93\"),\
(\"d4b3485d-7ef2-4ae2-b363-264e2fa2ae95\"),\
(\"df051319-8d73-4ee7-8e4a-0d55d598e39e\"),\
(\"8cbe748b-ab90-4346-bd0b-e898a9c1fb61\"),\
(\"5ddf4434-f0de-49ab-9735-c703ddeec107\"),\
(\"2c0e3146-6342-4075-8f05-34d097ec51b3\"),\
(\"ea8c924a-6381-4e3d-a0c4-c9bc48177b5d\"),\
(\"fd2befa8-666c-4d23-82d9-faff05f93922\"),\
(\"260d7d05-6564-4aef-98ea-1bc24a778a7c\"),\
(\"4541475b-adb0-4f78-99e1-5c0e6ecbdd73\"),\
(\"c032fdb9-9455-488c-be60-c35bc605c9ba\"),\
(\"5459c688-7a6d-4793-98e3-ffc7d126aa1a\"),\
(\"17c0de84-8c74-4598-a7d8-003b7c7547be\"),\
(\"d46e956a-b633-46a9-8dbd-9fd082af3bc8\"),\
(\"f7cd8e36-d40a-4a6d-adeb-78c99042f6c0\"),\
(\"6ee08948-7e4a-4ca7-8346-93cd3d0731c7\"),\
(\"533f458b-0885-45b9-9c41-53ef17e1b7e0\"),\
(\"b25802d7-b334-4ea8-800e-6fa7fb943c68\"),\
(\"b24c72a2-9c07-4ef2-abe1-738bbc90cce0\"),\
(\"bcddd77f-d340-43d0-ac8c-3917ab162a31\"),\
(\"88258a3a-ea88-468b-9db9-3b5c7d8bfda9\"),\
(\"f02b4e4d-0536-409d-b0d5-4302f9336a07\"),\
(\"2c20676b-b3da-4f0e-902d-878c12ccdcda\"),\
(\"5fa51206-a80b-467a-b3d7-996c44ee5db9\"),\
(\"8c4311f1-edf7-4915-af56-1d9a7cf92925\"),\
(\"f84fa0cc-2dac-454b-886b-079e3514d2c3\"),\
(\"e56fed0e-833d-4197-b983-0f567cb23e4b\"),\
(\"518eba3d-f12f-4a95-8972-e1e90cc7905d\"),\
(\"ade60e4b-dfde-4e31-8435-adf02306efe4\"),\
(\"2d3af7f3-b684-4cfa-84e1-b8695bddf649\"),\
(\"73f031dd-2002-4f37-b75c-8a8cd63dba24\"),\
(\"e59a7901-8470-4347-ab89-d952a727501a\"),\
(\"328b201a-9924-4b05-b138-e4d778e70fb6\"),\
(\"406509ea-eb9b-458d-a6be-eadf3c734c8a\"),\
(\"ca3873be-859c-44b1-b3f7-a6f11cb1cea7\"),\
(\"2b332eeb-0048-449a-9a4b-df471ac9c75d\"),\
(\"288f6e28-b114-40e7-81cd-4a848ff02caa\"),\
(\"ec7e80f2-292b-42e6-8672-583c5b10d312\"),\
(\"15a0cda7-4ec1-4a6e-a056-e0b443601616\"),\
(\"b34d99b6-ea5c-476e-b6e0-37904fa456ed\"),\
(\"8079e1cc-587f-4751-870f-f99f96eba257\"),\
(\"fc32897c-813d-4e9e-99a6-f7dcd7eaaa3d\"),\
(\"35be2313-74b2-4923-968b-21c01e117602\"),\
(\"e08f64e8-b7f1-4d11-bbc1-c0be8fd20eb0\"),\
(\"68636d59-4c57-4ce8-9880-b3d13eb06ce1\"),\
(\"2826f700-5985-4088-a812-063d7ed1d3e9\"),\
(\"862a5ea1-1464-4168-b0dc-016171617e4f\"),\
(\"62585c8a-f219-4a8c-9369-90fc6dbe3133\"),\
(\"83bdca3a-d7d5-482a-93ed-e8ef96f4b1e5\"),\
(\"6c2cd195-82a5-4992-ba32-4751b68dd38e\"),\
(\"815044d8-4f8d-475e-931a-c07a2ec489c0\"),\
(\"4f5244c1-c6e5-45d9-bdf1-7e3a88e0bd60\"),\
(\"80dac462-024c-4ed5-8b18-26790545beea\"),\
(\"0513ec07-e960-44ee-af1e-c52060f4e7c9\"),\
(\"ce629c74-ef2a-468d-932b-f38522d27099\"),\
(\"b7f7a06d-c823-4833-81fc-a8164195f5a6\"),\
(\"a5bffb29-b068-4376-a856-dfd657e12337\"),\
(\"9f66c317-0077-4771-a597-c77698baef61\"),\
(\"53dd0be2-66c1-4e96-a006-f3f71a39a17e\"),\
(\"1426094a-f354-4fd8-b198-0ccbc389efaa\"),\
(\"695c3092-9bfa-474f-a524-c0dc1c49d929\"),\
(\"a96eee0e-d804-4f26-be6a-b149e9ca0ee6\"),\
(\"bad9d8fc-916a-4617-9b09-37452709f429\"),\
(\"dfd8ce99-1921-4dc3-bf90-d5cc2e289015\"),\
(\"d4ac1074-8623-4c69-9221-c9e4469e65a4\"),\
(\"22d3df3e-5087-4ed9-b79f-3f6020ea3d1d\"),\
(\"1aa2ebee-aea8-49ad-834a-4b38f233d182\"),\
(\"18730eb5-e640-46be-a925-08a1f80f4133\"),\
(\"28b109f0-4a3a-4849-be4c-ace81f28a707\"),\
(\"e6fbf5a1-c6d0-413e-ae7a-55fdfabd9bb9\"),\
(\"1acf2814-3b4d-492b-a693-a3937a71a328\"),\
(\"e2cd3f40-8d61-4e85-b706-b65044bbb53b\"),\
(\"3c58de78-400c-4a43-92ce-10bc5b89aeb2\"),\
(\"46ee5ee4-8838-49e8-b9ad-ff11f5f4c91d\"),\
(\"06363d7e-f3ce-4a4d-a183-a82dad5b63b0\"),\
(\"5fa1b84a-6f5e-4b81-8d27-59b74dede4c6\"),\
(\"7b8460f8-84c5-4542-9ab6-b3f141accef6\"),\
(\"c409c476-d9dd-4a53-b4dd-de1020f6ba96\"),\
(\"27354dbf-3eba-498f-bd6a-1ec3ad6e405b\"),\
(\"26da5107-1207-443d-8ec6-99474ab4cd2f\"),\
(\"c59742fd-2462-4ef8-84ae-7e88dea7b7c8\"),\
(\"e4495a6b-8a52-4ad6-adfb-bb9e6596cb19\"),\
(\"9cb030db-38ed-433a-90c6-c5ecbbe27cb8\"),\
(\"e0046a12-24a1-4406-a492-6e105531004c\"),\
(\"4bfef242-3cfc-49b9-a3ce-69ce166b824b\"),\
(\"28f156f6-b9e3-47c4-8214-13408b750deb\"),\
(\"f02cc543-46b9-4dd6-8fdc-2bf3029d6839\"),\
(\"6ded13c4-3597-4a46-9f64-6540140ad46b\"),\
(\"628b7f67-d929-49c6-9e0f-5eb2efe0f325\"),\
(\"45db7e13-5342-4d1d-a7e4-c9f36e3db93e\"),\
(\"ffb761a2-a4f1-4bfa-be62-03c677daf406\"),\
(\"ae3cb93a-fa41-4e24-8801-00f6634fe8b9\"),\
(\"dbcfef84-ba08-4cc7-8314-ec107b6a5609\"),\
(\"f2b0d746-b370-4f23-baee-2880cf6e2e63\"),\
(\"68ac97bd-89ba-4bd0-81b7-cc7d371433c1\"),\
(\"2011065f-ffbf-485e-bc00-e95a6762de27\"),\
(\"188c2432-820d-4cdd-b69c-e91efabd5af4\"),\
(\"391b936d-dbaa-4217-9010-5e1b50273587\"),\
(\"dc2c105c-78f2-4057-95c6-c02d702e5e85\"),\
(\"92b6eb36-d3f4-4cf5-9370-82f3020cd740\"),\
(\"bee0e6a8-80f7-42a5-bd2a-3d54db7948c6\"),\
(\"37d498aa-8e96-4aa0-8b56-ae02e6014dd8\"),\
(\"34f4b275-f73d-4a83-9f36-b421626ac323\"),\
(\"6e3d2a4f-0fd1-4e20-870f-8402851314f9\"),\
(\"02b910ac-7ac4-4c68-9592-192823c79b29\"),\
(\"d5340305-d9a5-4a10-882a-1bea34f5b347\"),\
(\"e2273c50-0191-4274-9de4-9f332f00ae69\"),\
(\"34657b81-9b88-43b9-9a6e-47580404502c\"),\
(\"508b49c9-8543-4c71-94c7-879912c6a403\"),\
(\"de83a410-3040-4817-b988-14c819c6f59a\"),\
(\"3a8fe07a-ca4f-447d-8605-5f7ab7c3be58\"),\
(\"35bdad2f-c11b-4f92-bad9-9c772f96a9b6\"),\
(\"b8deb2bd-1d63-4799-a772-a24e2cb98ba3\"),\
(\"c786930b-5132-4ab1-8691-f6ac3e48b245\"),\
(\"719d166d-2a4a-4fc2-b209-d4ba1944de16\"),\
(\"758e4c90-183a-45a3-a4c9-170ed194b983\"),\
(\"30cf94bb-575c-4ad0-adb4-41d030a61c33\"),\
(\"0eefa18a-d69b-4496-983f-6fa83b6432ce\"),\
(\"65b75342-fa53-4769-b3c5-069615eefe2c\"),\
(\"06100ca2-3df1-4c23-8522-5a3200dc1c7f\"),\
(\"398fdce5-c259-4cf3-8f9a-c29034b32ed1\"),\
(\"9f850a13-cc1c-4af7-b0a9-7f58413d99a0\"),\
(\"dace54cf-ff29-4222-908d-4237d086d662\"),\
(\"a4848973-5b05-4244-a91c-f95fa06b56cb\"),\
(\"afa2837d-5b8b-447a-88e5-94fedf748323\"),\
(\"b8560fd7-2242-4a64-af8a-0163f3ed0b61\"),\
(\"5eabdcec-e277-4f08-8c5e-0d1ce7016079\"),\
(\"667b5ac7-41c3-47e3-80f6-089191371e0e\"),\
(\"9d8b7ecc-91c7-411c-837f-4509256207ea\"),\
(\"8371a6a1-3613-47ad-9862-b104f8e9f5a1\"),\
(\"cf7da1aa-bab5-4bdf-9530-54f11f63ab6d\"),\
(\"6b26c232-c93b-436c-b05a-31a92087288b\"),\
(\"c3f629de-cafc-48ea-9752-8c63c5c30659\"),\
(\"b8b3cb29-c7b8-4a8a-b09e-26a692eed67e\"),\
(\"8f8beb99-d5f5-42b4-b7f1-4acac3171b63\"),\
(\"0405fa9e-d81b-4b08-b4e5-930089f5bb81\"),\
(\"5eeb2401-0797-44e2-8e24-81ec95a9114f\"),\
(\"6715cbe5-636d-4a72-aa71-0d82dcc20a06\"),\
(\"62966b92-f06d-4158-88cd-06b9ce6c4590\"),\
(\"7b440008-8ce1-4c39-8297-acf9c4e25458\"),\
(\"f7db37ad-2249-4b81-8efb-2031adb71f37\"),\
(\"b362e1a5-1d1b-4e3c-b864-8d203f4219ce\"),\
(\"75cf1cb1-f3b8-4be9-bb77-a7ea3a29bc6a\"),\
(\"999cf7ac-5fc4-4f8d-bf48-a9fd41d16851\"),\
(\"88247a67-71ce-494c-9aba-621529abdce4\"),\
(\"badae1bf-2b82-44e3-bca7-f5387e70c5e1\"),\
(\"9b942b54-697b-4bca-94df-e43d9897387d\"),\
(\"6f85e86f-51f2-4b90-a59c-77cc2e6d6965\"),\
(\"e5e996c8-94c8-4b5f-81d4-c91850505bef\"),\
(\"a8144da2-bc42-4bdf-b6b5-50e3dc77e95d\"),\
(\"99dc11a3-ec0a-4d13-9cdc-9aa9eab4559a\"),\
(\"a643449b-651c-417a-aab3-b3e5fa035277\"),\
(\"7045abe2-074c-44b5-9db6-b4966667b229\"),\
(\"e895a33a-5b11-4201-9cc8-57b5429c0f12\"),\
(\"9c57c677-4973-4bc8-ab20-491b00d44e3b\"),\
(\"5b517a07-8d6d-47fc-9c36-a297a8d4df74\"),\
(\"cac0b967-a34f-481b-81db-34a803236aac\"),\
(\"fc94761c-7c49-431e-8eed-b8ddf2c7e1f8\"),\
(\"2f72571e-f598-463b-8fb1-562a581dfe94\"),\
(\"9c578376-0c4d-4c2a-ae1f-3e22d2681f6f\"),\
(\"595c05c0-3aba-4831-84b6-b1b03583e261\"),\
(\"cc1024fe-c181-4f29-b23b-ac22e6510bdb\"),\
(\"afc70809-f014-4fc7-8bad-650fc1e8e837\"),\
(\"dccefe63-ce54-46fb-a9cb-ca530b54d18f\"),\
(\"25a5986d-0567-4882-a2a0-bd2cf4c771e6\"),\
(\"92418d90-f247-4c98-a3f1-cd077015053f\"),\
(\"657875d1-44f6-43e9-addc-a845b38ba299\"),\
(\"a2f5b009-0a4a-46bf-a2c1-efd51c38fa08\"),\
(\"8c3bc43b-1bcb-47fa-a196-2bb9f65322bf\"),\
(\"026fac7f-8bb6-4618-a091-c87eb1eae86e\"),\
(\"200659db-c78b-44ad-a1a3-203cb985fd87\"),\
(\"0eb75aeb-ebdc-48eb-baeb-3510bfb7062e\"),\
(\"336b0ebb-3317-4234-bb3b-b7bcb9161a82\"),\
(\"3f65f97f-da3c-4cc6-8375-4dff89e25f89\"),\
(\"47d3eff1-9ea8-490c-ae4a-619aa4fbc85a\"),\
(\"6d4d4e03-fea6-412e-b323-8de61cf21c7c\"),\
(\"61002b2b-3e14-4cc2-b002-ebdf142a5f5c\"),\
(\"95862e8e-e5b2-4e7b-aedc-db5fe09eff1e\"),\
(\"805bf9e2-dab0-4e06-9635-753921147776\"),\
(\"863faaa4-137e-4ac9-831e-aa531e4f7d68\"),\
(\"5f39c105-85b6-4527-9a46-1765bf6ec6b9\"),\
(\"a8f3c2ff-8f0e-43df-9043-3270c4c49846\"),\
(\"f27e0cfb-e813-4586-a7cc-46b689cbfdaa\"),\
(\"27d4b9e1-bee1-44f5-ad83-dc11f1f6e387\"),\
(\"894a34a9-62d7-487b-8328-71a26dcc8f2e\"),\
(\"42267c66-f790-43f7-bb6f-42109137758d\"),\
(\"363f1509-fdff-4756-875a-a9fdb8bee40e\"),\
(\"370c96d8-1284-44d7-a2ff-01ff9df299fe\"),\
(\"f8883a99-49a2-41b6-be84-68071b8106d6\"),\
(\"2bb2bb99-a87a-4329-a6a3-453070fe3b60\"),\
(\"4a26c38c-2c5e-43b1-a95d-26e5a75b2f3b\"),\
(\"c9987211-b7e2-46eb-874e-3235e8eebfe4\"),\
(\"98764265-0e97-4cfd-bbbc-a8b6242abfa9\"),\
(\"af7935ce-c49a-4653-84ab-dc0643d5573c\"),\
(\"11cac1ce-0b7f-49d7-922f-0acee428887e\"),\
(\"ef200675-b27b-4ca6-92a1-adca12a6cc90\"),\
(\"e2346ecc-18d5-4982-b5d6-abf4f63ed4a6\"),\
(\"dace27ba-84ee-46e7-9105-b989953c1e5c\"),\
(\"26a83513-9ce1-4e34-8874-c95fe8145f3b\"),\
(\"f16c3e6e-8ffb-4d72-8abe-47d0faec95aa\"),\
(\"4f557147-1561-40e7-91ba-70524ccdda94\"),\
(\"065aa3e8-d9eb-40a9-96b4-aeb5b6a24478\"),\
(\"f11308ce-3059-4031-8858-4b89ee9b8058\"),\
(\"78238669-52c6-488a-bcd0-43f4c9bab4c7\"),\
(\"3138b148-4980-46ef-8118-e5506fd1a06c\"),\
(\"9634978d-f371-4721-8a16-4d1e34bae8ce\"),\
(\"a8b06185-ef6f-41e1-845e-c89c3aa83c84\"),\
(\"13150a27-fe3a-4a56-8a7f-b667d94d25da\"),\
(\"6fae6cd0-1119-4684-b433-27fee57aabaf\"),\
(\"e41ec698-75f9-426c-9991-765f8480544d\"),\
(\"bded059e-b770-4211-ad73-bfb66ed53dca\")");

  OK_SIMPLE_STMT(Stmt, "DROP TABLE `t_odbc_replace`");

  return OK;
}


MA_ODBC_TESTS my_tests[]=
{
  {t_disconnect, "t_disconnect",      NORMAL},
  {t_describe_nulti, "t_describe_nulti", NORMAL},
  {test_CONO1,     "test_CONO1",     NORMAL},
  {test_CONO3,     "test_CONO3",     NORMAL},
  {t_count,        "t_count",        NORMAL},
  {simple_test,    "Simple test",    NORMAL},
  {simple_test1,   "Simple test1",   NORMAL},
  {select1000,     "select1000",     NORMAL},
  {simple_2,       "simple_2",       NORMAL},
  {test_reconnect, "test_reconnect", NORMAL},
  {bug19823,       "bug19823",       NORMAL},
  {t_basic,        "t_basic",        NORMAL},
  {t_reconnect,    "t_reconnect",    NORMAL},
  {charset_utf8,   "charset_utf8",   NORMAL},
  {charset_gbk,    "charset_gbk",    NORMAL},
  {t_bug30774,     "t_bug30774",     NORMAL},
#ifdef WE_HAVE_SETUPLIB
  {t_bug30840,     "t_bug30840",     NORMAL},
#endif
  {t_bug30983,     "t_bug30983",     NORMAL},
  {t_driverconnect_outstring, "t_driverconnect_outstring", NORMAL},
  {setnames,       "setnames",       NORMAL},
  {setnames_conn,  "setnames_conn",  NORMAL},
  {sqlcancel,      "sqlcancel",      NORMAL}, 
  {t_bug32014,     "t_bug32014",     NORMAL},
  {t_bug10128,     "t_bug10128",     NORMAL},
  {t_bug32727,     "t_bug32727",     NORMAL},
  {t_bug28820,     "t_bug28820",     NORMAL},
  {t_bug31959,     "t_bug31959",     NORMAL},
  {t_bug41256,     "t_bug41256",     NORMAL},
  {t_bug48603,     "t_bug48603",     NORMAL},
  {t_bug45378,     "t_bug45378",     NORMAL},
  {t_mysqld_stmt_reset, "tmysqld_stmt_reset bug", NORMAL},
  {t_odbc32,      "odbc32_SQL_ATTR_PACKET_SIZE_option", NORMAL},
  {t_gh_issue3,   "leading_space_gh_issue3",  NORMAL},
  {t_odbc48,      "odbc48_iso_call_format",   NORMAL},
  {t_odbc69,      "odbc69_ci_connstring",     NORMAL},
  {t_odbc91,      "odbc91_hdbc_reuse",        NORMAL},
  {t_odbc137,     "odbc137_ansi",             NORMAL},
#ifdef _WIN32
  {t_odbc139,     "odbc139_compression",       NORMAL},
#endif
  {t_odbc162,     "t_odbc162_CTE_query",      NORMAL },
  {t_replace,     "t_replace",      NORMAL },
  {NULL, NULL, 0}
};

int main(int argc, char **argv)
{
  int tests= sizeof(my_tests)/sizeof(MA_ODBC_TESTS) - 1;
  get_options(argc, argv);
  plan(tests);
  return run_tests(my_tests);
}
