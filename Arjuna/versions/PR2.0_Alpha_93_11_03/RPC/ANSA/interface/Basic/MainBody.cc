/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: MainBody.cc,v 1.1 1993/11/03 14:38:52 nmcl Exp $
 */

extern "C"
{
extern int nucleus_main(int argc, char **argv, char **envp);
extern int mainbody__FiPPc(int argc, char **argv);

int body ( int argc, char **argv )
{
	mainbody__FiPPc(argc, argv);

	return 0;
}

};

int main ( int argc, char **argv, char **envp)
{
	return nucleus_main(argc, argv, envp);
}
