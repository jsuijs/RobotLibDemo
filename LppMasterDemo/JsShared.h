//-----------------------------------------------------------------------------
// my_putc - links stdout (printf) to serial
//-----------------------------------------------------------------------------
// put 'fdevopen( &my_putc, 0);' in setup();
//-----------------------------------------------------------------------------
int my_putc(char c, FILE *t) {
  if (c == '\n') Serial.write('\r');
  return Serial.write(c);
}

int I2cDebug = 0;

//-----------------------------------------------------------------------------
// I2cSendReceive - Send and/or Receive data to/from i2c slave.
//-----------------------------------------------------------------------------
// return: true on success
//-----------------------------------------------------------------------------
bool I2cSendReceive(byte I2cSlaveAddress, byte TxCount, byte RxCount, const byte *TxBuffer, byte *RxBuffer)
{
  byte r;

  if (I2cDebug) {
    printf("I2cSendReceive(%d %d %d)\n", I2cSlaveAddress, TxCount, RxCount);
  }

  if (TxCount > 0) {
    if (I2cDebug > 1) {
      printf("TxBuf:\n");
      HexDump(TxBuffer, TxCount);
    }

    Wire.beginTransmission(I2cSlaveAddress);
    for (int i=0; i<TxCount; i++) {
      Wire.write(TxBuffer[i]); //
    }
    r = Wire.endTransmission();
    if (r != 0) return false;  // error
  }

  if (RxCount > 0) {

    for (int i=0; i<RxCount; i++) RxBuffer[i] = 0;

    Wire.requestFrom(I2cSlaveAddress, RxCount);    // request byte(s) from slave device

    if (Wire.available() != RxCount) return false;  // error

    for (int i=0; i<RxCount; i++) {
      RxBuffer[i] = Wire.read();
    }
    if (I2cDebug > 1) {
      printf("RxBuf:\n");
      HexDump(RxBuffer, RxCount);
    }
  }
  return true;  // success
}


/*=====================================================================
 HexDump :
 ---------------------------------------------------------------------*/
// Parameters:
//    Data   - data to be dumped
//    Length - nr of bytes to be dumped
//    Offset - offset of address (from 0), displayed at the start of each line.
//-----------------------------------------------------------------------------
void HexDump( const void *Data, unsigned int Length, unsigned int Offset)
{
  unsigned char *data    = (unsigned char *)Data    ;

  unsigned int Track1 = 0 ;
  unsigned int Track2 = 0 ;

  for( unsigned int Index=0 ; Index < Length ; Index = Index+16 )
  {
    printf( "%04x: ", Offset + Index ) ;

    for( unsigned int j=0 ; j < 16 ; j++ )
    {
      if( Track1 < Length ) printf( "%02x", data[ Index+j ] ) ;
      else printf( "  " ) ;

      printf( " " ) ;

      Track1++ ;
    }

    printf( " "  ) ;

    for( unsigned int j=0 ; j < 16 ; j++ )
    {
      if( Track2 < Length )
      {
        if( data[ Index+j ] < 32 ) printf( "." ) ;
        else
        {
          if( data[ Index+j ] < 127 ) printf( "%c", data[ Index+j ] ) ;
          else printf( "."                   ) ;
        }
      }
      else printf( " " ) ;

      Track2++ ;
    }

    printf( "\r\n" ) ;
  }
}


