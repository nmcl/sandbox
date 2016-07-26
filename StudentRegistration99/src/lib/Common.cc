/*
 * Provides those routines which DBAccess-utils.c and DBAccess-utils.cc share.
 */

#include <System/string.h>
#include <System/memory.h>

char* createFileName (const char* path, const char* name)
{
    char* ptr = ::new char[::strlen(path)+::strlen(name)+1];
    ::strcpy(ptr, path);
    ::strcat(ptr, name);

    return ptr;
}

void CreateBlankRec( char* Record )
{
	char *Surname = "", *OtherNames = "", *StudType = "U",
	*CourseType = "T", *MStat = "S", *Sex = "M", *Title = "",
	*Initials = "", *DOB = "", *MOB = "", *YOB = "", *National = "2826",
	*BirthCountry = "2826", *Domicile = "5826", *RStat = "H",
	*FeeStat = "H", *XTime = "F", *XYear = "F", *Reside = "5", *Occup = "",
	*HAdd1 = "", *HAdd2 = "", *HAdd3 = "", *HAdd4 = "", *HAdd5 = "",
	*HPostcode = "", *HTelephone = "", *TTAdd1 = "", *TTAdd2 = "",
	*TTAdd3 = "", *TTAdd4 = "", *TTAdd5 = "", *TTPostcode = "",
	*TTTelephone = "", *NOKSurname = "", *NOKForename = "", *NOKRel = "",
	*NOKAdd1 = "", *NOKAdd2 = "", *NOKAdd3 = "",*NOKAdd4 = "",
	*NOKAdd5 = "", *NOKPostcode = "", *NOKTelephone = "", *StudyCourse = "",
	*Stage = "", *GAB = "", *TuitFee = "", *Tutor = "        ",
	*Ethnic = "99", *Disabled = "9", *Disability = "98", *UKCounty = "",
	*TutDept = "0000", *LocalAddr = "99", *DateStamp = "00000000003600  T",
	*AccessState = "D", *Spare2 = "", *Modules = "", *Blank = "", 
	*DebtType = "", *SDebt = "", *Newstat = "B";

	memcpy( &Record[ 9 ], Surname, strlen( Surname ));
	memcpy( &Record[ 47 ], OtherNames, strlen( OtherNames ));
	memcpy( &Record[ 112 ], StudType, strlen( StudType ));
	memcpy( &Record[ 113 ], CourseType, strlen( CourseType ));
	memcpy( &Record[ 114 ], MStat, strlen( MStat ));
	memcpy( &Record[ 115 ], Sex, strlen( Sex ));
	memcpy( &Record[ 116 ], Title, strlen( Title ));
	memcpy( &Record[ 120 ], Initials, strlen( Initials ));
	memcpy( &Record[ 124 ], DOB, strlen( DOB ));
	memcpy( &Record[ 126 ], MOB, strlen( MOB ));
	memcpy( &Record[ 128 ], YOB, strlen( YOB ));
	memcpy( &Record[ 132 ], National, strlen( National ));
	memcpy( &Record[ 136 ], BirthCountry, strlen( BirthCountry ));
	memcpy( &Record[ 140 ], Domicile, strlen( Domicile ));
	memcpy( &Record[ 144 ], RStat, strlen( RStat ));
	memcpy( &Record[ 145 ], FeeStat, strlen( FeeStat ));
	memcpy( &Record[ 146 ], XTime, strlen( XTime ));
	memcpy( &Record[ 147 ], XYear, strlen( XYear ));
	memcpy( &Record[ 148 ], Reside, strlen( Reside ));
	memcpy( &Record[ 149 ], Occup, strlen( Occup ));
	memcpy( &Record[ 152 ], HAdd1, strlen( HAdd1 ));
	memcpy( &Record[ 182 ], HAdd2, strlen( HAdd2 ));
	memcpy( &Record[ 212 ], HAdd3, strlen( HAdd3 ));
	memcpy( &Record[ 242 ], HAdd4, strlen( HAdd4 ));
	memcpy( &Record[ 272 ], HAdd5, strlen( HAdd5 ));
	memcpy( &Record[ 302 ], HPostcode, strlen( HPostcode ));
	memcpy( &Record[ 310 ], HTelephone, strlen( HTelephone ));
	memcpy( &Record[ 330 ], TTAdd1, strlen( TTAdd1 ));
	memcpy( &Record[ 360 ], TTAdd2, strlen( TTAdd2 ));
	memcpy( &Record[ 390 ], TTAdd3, strlen( TTAdd3 ));
	memcpy( &Record[ 420 ], TTAdd4, strlen( TTAdd4 ));
	memcpy( &Record[ 450 ], TTAdd5, strlen( TTAdd5 ));
	memcpy( &Record[ 480 ], TTPostcode, strlen( TTPostcode ));
	memcpy( &Record[ 488 ], TTTelephone, strlen( TTTelephone ));
	memcpy( &Record[ 508 ], NOKSurname, strlen( NOKSurname ));
	memcpy( &Record[ 538 ], NOKForename, strlen( NOKForename ));
	memcpy( &Record[ 568 ], NOKRel, strlen( NOKRel ));
	memcpy( &Record[ 591 ], NOKAdd1, strlen( NOKAdd1 ));
	memcpy( &Record[ 621 ], NOKAdd2, strlen( NOKAdd2 ));
	memcpy( &Record[ 651 ], NOKAdd3, strlen( NOKAdd3 ));
	memcpy( &Record[ 681 ], NOKAdd4, strlen( NOKAdd4 ));
	memcpy( &Record[ 711 ], NOKAdd5, strlen( NOKAdd5 ));
	memcpy( &Record[ 741 ], NOKPostcode, strlen( NOKPostcode ));
	memcpy( &Record[ 749 ], NOKTelephone, strlen( NOKTelephone ));
	memcpy( &Record[ 769 ], StudyCourse, strlen( StudyCourse ));
	memcpy( &Record[ 773 ], Stage, strlen( Stage ));
	memcpy( &Record[ 774 ], GAB, strlen( GAB ));
	memcpy( &Record[ 780 ], TuitFee, strlen( TuitFee ));
	memcpy( &Record[ 789 ], Tutor, strlen( Tutor ));
	memcpy( &Record[ 797 ], Ethnic, strlen( Ethnic ));
	memcpy( &Record[ 799 ], Disabled, strlen( Disabled ));
	memcpy( &Record[ 800 ], Disability, strlen( Disability ));
	memcpy( &Record[ 802 ], UKCounty, strlen( UKCounty ));
	memcpy( &Record[ 806 ], TutDept, strlen( TutDept ));
	memcpy( &Record[ 810 ], LocalAddr, strlen( LocalAddr ));
	memcpy( &Record[ 812 ], DateStamp, strlen( DateStamp ));
	memcpy( &Record[ 831 ], AccessState, strlen( AccessState ));
	memcpy( &Record[ 832 ], Modules, strlen( Modules ));
	memcpy( &Record[ 988 ], Blank, strlen( Blank ));
	memcpy( &Record[ 1000 ], DebtType, strlen( DebtType ));
	memcpy( &Record[ 1001 ], SDebt, strlen( SDebt ));
	memcpy( &Record[ 1010 ], Newstat, strlen( Newstat ));
}
