/*
 * Gallery - a web based photo album viewer and editor
 * Copyright (C) 2000-2006 Bharat Mediratta
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
/**
 * @version $Revision: 12811 $ $Date: 2006-01-28 18:31:08 -0500 (Sat, 28 Jan 2006) $
 * @package Gallery
 * @author Larry Menard
 */

/*
 * g2_db2_bit_or:
 *
 * A Java User-Defined Function (UDF) to implement BIT_OR functionality for DB2.
 *
 * Developed with much thanks to:
 *
 *    - http://www-128.ibm.com/developerworks/db2/library/techarticle/0309stolze/0309stolze.html
 *
 * and
 *
 *    - "A Complete Guide to DB2 Universal Database" by Don Chamberlin
 *       (Morgan Kaufmann Publishers Inc, 1998, ISBN 1-55860-482-0)
 *
 * and of course, the DB2 UDB documentation and samples:
 *
 *    - http://publib.boulder.ibm.com/infocenter/db2luw/v8//index.jsp
 */

import java.lang.*;          // for String class
import java.io.*;            // for ...Stream classes
import COM.ibm.db2.app.*;    // UDF and associated classes

public class g2_db2_bit_or extends UDF
{
  public void g2_db2_bit_or ( String inVar,
                          Blob intermResult )
  throws Exception
  {
    // Test for SQL NULLs in the input parameters and
    // the structured value itself
    if (isNull(1))
    {
        return;
    }

    // variables to read from SCRATCHPAD area
    byte[] scratchpad = getScratchpad();
    ByteArrayInputStream byteArrayIn =
            new ByteArrayInputStream(scratchpad);
    DataInputStream dataIn =
            new DataInputStream(byteArrayIn);

    // variables to write into SCRATCHPAD area
    ByteArrayOutputStream byteArrayOut =
            new ByteArrayOutputStream(32);
    DataOutputStream dataOut =
            new DataOutputStream(byteArrayOut);

    // Initialize variables
    // Byte buffer for the scratchpad copy of the value to be BIT_ORed
    byte[] workBuffer = new byte[32];
    int ctr = 0;

    // Get the current scratchpad into the work buffer
    dataIn.read(workBuffer, 0, 32);

    // Convert the column value string to an array of bytes
    byte[] columnValueArray = new byte[32];
    columnValueArray = inVar.getBytes();

    // Walk the inVar byte array, test each char.
    for (ctr = 0; ctr <= 31; ctr++)
    {
       if (workBuffer[ctr] != 49)
       {
          // Check the current column value byte.
          if (columnValueArray[ctr] == 49)          // If it's 1 (ASCII 49 = numeric 1),
          {
             workBuffer[ctr] = 49;                  // then set the corresponding scratchpad bit to 1,
          }
          else
          {
             workBuffer[ctr] = 48;                  // else set the corresponding scratchpad bit to 0.
          }
       }
    }

    // Write the updated buffer to dataOut
    dataOut.write(workBuffer, 0, 32);

    // Construct new scratchpad data and store it
    for (ctr = 0; ctr < workBuffer.length; ctr++)
    {
       scratchpad[ctr] = workBuffer[ctr];
    }
    setScratchpad(scratchpad);

    // Set output parameter for new intermediate result
    // (VARCHAR FOR BIT DATA is mapped to "Blob" class)
    intermResult = Lob.newBlob();
    OutputStream intermOut = intermResult.getOutputStream();
    intermOut.write(workBuffer);
    set(2, intermResult);
  }
}

