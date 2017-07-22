%{
#include <stdio.h>

int state;

int main()
{
   printf("HyperText\n200\n?? Num Words ??\n", yytext);
   state = 0;
   yylex();
}

int  yywrap()
{
    return 1;
}

%}

%%

[A-Za-z0-9\:\;\,\.\-\_]* { switch (state)
                         {
                         case 0:
                             printf("%s 0\n", yytext);
                             state = 0;
                             break;
                         case 1:
                             printf("%s 1 ", yytext);
                             state = 2;
                             break;
                         case 2:
                             printf("%s ", yytext);
                             state = 3;
                             break;
                         case 3:
                             printf("%s\n", yytext);
                             state = 4;
                             break;
                         default:
                             printf("Error [%s], state: [%d]\n", yytext, state);
                             exit(2);
                             break;
                         };
                       }

\{                     { switch (state)
                         {
                         case 0:
                             state = 1;
                             break;
                         default:
                             printf("Error [%c], state: [%d]\n", yytext[0], state);
                             exit(2);
                             break;
                         };
                       }

\}                     { switch (state)
                         {
                         case 4:
                             state = 0;
                             break;
                         default:
                             printf("Error [%c], state: [%d]\n", yytext[0], state);
                             exit(2);
                             break;
                         };
                       }

[ \t\n]                { ; }

.                      { printf("Error [%c] : %d\n", yytext[0], (int) yytext[0]);
                         exit(1); }

%%
