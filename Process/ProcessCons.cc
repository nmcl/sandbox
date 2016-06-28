#include "ProcessCons.h"

ProcessCons::ProcessCons(Process &p, ProcessCons *n) { Proc = &p; Next = n; }

Process *ProcessCons::car() { return Proc; }

ProcessCons *ProcessCons::cdr() { return Next; }

void ProcessCons::SetfCdr(ProcessCons *n) { Next = n; }
