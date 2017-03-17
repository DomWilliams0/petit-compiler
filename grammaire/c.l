N [0-9]
IC [a-zA-Z_]
C .


%{
#include <iostream>
#include "c.tab.h"
%}

%option noyywrap

%%

"#"[^\n]*		{ /* ignore */ }

N+        { return CONSTANT; }
\\?C      { return CONSTANT; }
IC[IC|N]* { return IDENTIFIER; }


%%
