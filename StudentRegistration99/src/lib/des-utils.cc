#include <System/string.h>
#include <System/stdio.h>

#ifndef DEFS_H_
#  include "Defs.h"
#endif

#define HEXBASE (int)'a'
#define BUFFLEN 10240

char ToBit( register char character )
{
	return ( character - ((int)character - HEXBASE >= 0 ? HEXBASE - 10 : (int)'0' ));
};

char ToHex( register char character )
{
	return ( character + ( character > 9 ? HEXBASE - 10: '0' ));
}

/*
  Convert a sequence of bits into HexString equiv.  Length is the number of
  bytes to the Bytes parameter.

  IE.
  0001 1000 1100 1001
  would give "18d9" and Length = 2
*/

char *BytesToHexString( char* Bytes, char* HexString, int Length )
{
	int ByteNumber;

	memset( HexString, 0, Length * 2 );
	for( ByteNumber = 0; ByteNumber < Length ; ByteNumber++ )
	{
		HexString[ ByteNumber * 2 ] = ToHex( (unsigned)Bytes[ ByteNumber ] /16 & 15);
		HexString[ ByteNumber * 2 + 1 ] = ToHex( (unsigned)Bytes[ ByteNumber] & 15 );
	}

	return HexString;
}

/*
  HexStringToBytes converts a HexString in to the byte equiv.  Length is the
  counter of bytes expected in the Bytes parameter.

  IE.
  "a1c5e7"
  would give 1010 0001 1100 0101 1110 0111 and Length = 3
*/

char *HexStringToBytes( char* HexString, char* Bytes, int Length )
{
	int ByteNumber;
	memset( Bytes, 0, Length );

	for( ByteNumber = 0; ByteNumber < Length; ByteNumber++ )
		Bytes[ ByteNumber ] = (char)
			( ToBit((int)HexString[ ByteNumber * 2 ]) * 16 +
			ToBit((int)HexString[ ByteNumber * 2 + 1 ]) );

	return Bytes;
}

/* This function will convert the given text string into a hex string */

char *TextStringToHexString( char* TextString, char* HexString )
{
	char	InternalHexString[ 17 ];

	memset( InternalHexString, 0, 17 );
	sprintf( InternalHexString, "%x%x%x%x%x%x%x%x",
                (int)TextString[ 0 ],(int)TextString[ 1 ],(int)TextString[ 2 ],
                (int)TextString[ 3 ],(int)TextString[ 4 ],(int)TextString[ 5 ],
                (int)TextString[ 6 ],(int)TextString[ 7 ] );

	memcpy( HexString, InternalHexString, 16 );
	
	return HexString;
}

/* This function will convert a hex string into a text string */

char *HexStringToTextString( char* HexString, char* TextString, int Limit )
{
	int Position;

	for( Position = 0; Position < Limit; Position++ )
	TextString[ Position ] =
		16 * ToBit( HexString[ Position * 2 ] ) + ToBit( HexString[ Position * 2 + 1 ]);

	return TextString;
}

char *EncodeBlock( char* In, char* Out, int Length )
{
	char *Pad = "        ", *CurrentIn, *CurrentOut, InLine[ BUFFLEN ],
		OutLine[ BUFFLEN ], TextHex[ 16 ], TextBit[ 8 ], DesBit[ 8 ],
		LastDesBit[ 8 ];
	unsigned int ProcessLoop;

	memcpy( memset( InLine, 0, BUFFLEN ), In, Length);
	memset( OutLine, 0, BUFFLEN );

	if( strlen( InLine ) % 8 != 0 )
		strcat( InLine, &Pad[ strlen( InLine ) % 8 ] );

	CurrentIn = InLine;
	CurrentOut = OutLine;

	/* Process the Hex digit prefix */

	endes( HexStringToBytes( CurrentIn, TextBit, 8 ), DesBit );
	BytesToHexString( DesBit, CurrentOut, 8 );

	memcpy( LastDesBit, DesBit, 8 );

	CurrentIn += 16; CurrentOut += 16;

	for( ProcessLoop = 16; ProcessLoop < strlen( InLine ); ProcessLoop += 8 )
	{
		memset( TextHex, 0, 16 );
		memset( TextBit, 0, 8 );

		TextStringToHexString( CurrentIn, TextHex );
		HexStringToBytes( TextHex, TextBit, 8);
		xor( LastDesBit, TextBit );

		endes( TextBit, DesBit );
		memcpy( LastDesBit, DesBit, 8 );
		BytesToHexString( DesBit, CurrentOut, 8 );

		CurrentIn += 8; CurrentOut += 16;
	}
	memcpy( Out, OutLine, strlen( OutLine ));
	return Out;
}

char *DecodeBlock( char* In, char* Out, int Length )
{
	char InLine[ BUFFLEN ], OutLine[ BUFFLEN ], *Pad = "        ", *CurrentIn,
		LastDesBit[ 8 ], *CurrentOut, DesBit[ 8 ], TextBit[ 8 ],
		TextHex[ 16 ], OutputString[ 17 ];
	int Position;

	memset( OutLine, 0, BUFFLEN );
	memset( OutputString, 0, 17 );
	memset( InLine, 0, BUFFLEN );
	memcpy( InLine, In, strlen( In ));
	if( strlen( InLine ) % 8 != 0 )
		strcat( InLine, &Pad[ strlen( InLine ) % 8 ] );

	CurrentIn = InLine;
	CurrentOut = OutLine;

	HexStringToBytes( CurrentIn, DesBit, 8 );
	dedes( DesBit, TextBit );
	BytesToHexString( TextBit, TextHex, 8 );

	memcpy( CurrentOut, TextHex, 16 );

	CurrentIn += 16; CurrentOut += 16;

	for( Position = 0; Position < Length - 16; Position += 16 )
	{
		memcpy( LastDesBit, DesBit, 8 );

		HexStringToBytes( CurrentIn, DesBit, 8);
		dedes( DesBit, TextBit );
		xor( LastDesBit, TextBit );

		HexStringToTextString( BytesToHexString( TextBit, TextHex, 8), CurrentOut, 8 );
		CurrentIn += 16; CurrentOut += 8;
	};

	memcpy( Out, OutLine, strlen( OutLine ));

	return Out;
}

