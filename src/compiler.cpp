#include <cstdio>
#include <iostream>
#include <fstream>
#include "getopt/getopt.h"
#include "Printer.h"
#include "Element.h"
#include "Expression.h"
#include "Statement.h"
#include <fstream>
#include "Interpreter.h"

extern int yyparse(Document *);
extern FILE *yyin;

struct Settings
{
	Settings() :
		staticAnalysis(false), optimisation(false),
		compile(false), compileOutFileSpecified(false),
		astTree(false), astOutFileSpecified(false) {}

	void printSelf() const
	{
		std::cout << "Compiling file '" << fileToCompile << "'";

		if (staticAnalysis)
			std::cout << ", static analysis";

		if (optimisation)
			std::cout << ", optimising";

		if (compile)
		{
			std::cout << ", compiling to ";
			if (compileOutFileSpecified)
				std::cout << "specified file '" << compileOutFile << "'";
			else
				std::cout << "default .asm file";
		}

		if (astTree)
		{
			std::cout << ", printing AST to ";
			if (astOutFileSpecified)
				std::cout << "specified file '" << astOutFile << "'";
			else
				std::cout << "default .png file";
		}

	};

	bool staticAnalysis;
	bool optimisation;
	bool compile;
	bool compileOutFileSpecified;
	std::string compileOutFile;

	bool astTree;
	bool astOutFileSpecified;
	std::string astOutFile;

	std::string fileToCompile;
};

int doWork(const Settings &settings);
void printUsage(char *arg0);

int main(int argc, char **argv)
{
	// ./c <fichier>
	// -a -> static analysis
	// -o -> optimisation
	// -c -> compile to <filename>.asm
	// --outfile <filename> -> defaults to filename.asm
	// -t -> ast.png
	// --astfile -> output file for ast.png
	Settings settings;

	option options[] = {
		{"outfile", required_argument, 0, 0},
		{"astfile", required_argument, 0, 0},
		{0, 0, 0, 0}
	};

	int optionIndex = 0;

	bool error = false;
	int i;
	while ((i = getopt_long(argc, argv, "aoct", options, &optionIndex)) != -1)
	{
		switch (i)
		{
			// long option
			case 0:
				switch (optionIndex) // god help us all
				{
					case 0:
						settings.compileOutFile = optarg;
						settings.compileOutFileSpecified = true;
						break;

					case 1:
						settings.astOutFile = optarg;
						settings.astOutFileSpecified = true;
						break;
				}
				break;

			case 'a':
				settings.staticAnalysis = true;
				break;

			case 'c':
				settings.compile = true;
				break;

			case 'o':
				settings.optimisation = true;
				break;

			case 't':
				settings.astTree = true;
				break;
			case '?':
				error = true;
				break;

		}
	}

	if (optind + 1 != argc)
	{
		std::cerr << "A single file to compile must be given" << std::endl;
		error = true;
	}

	if (error)
	{
		printUsage(argv[0]);
		return 1;
	}

	settings.fileToCompile = argv[optind];

	return doWork(settings);
}

void printUsage(char *arg0)
{
	static const char usage[] =
        "Un petit compiler.\n"
        "   Usage:\n"
        "       %s <file>\n"
        "\n"
        "   Options:\n"
        "       -a          Static analysis.\n"
        "       -o          Enable optimisation.\n"
        "       -c          Compile to <file>.asm, or the path specified by `--outfile` if given.\n"
        "       --outfile   Specify the output path for compilation.\n"
        "       -t          Generate a Graphviz dot graph of the AST in <file>.dot, or the path specified by `--astfile` if given.\n"
        "       --astfile   Specify the output path for AST graph\n"
        "\n";
	fprintf(stderr, usage, arg0);
}


void parseFileName(const std::string &originalFile, const std::string &replacement, const std::string &extension, std::ostream **out,
		bool &freeMe)
{
	*out = NULL;

	// stdout
	if (replacement == "-")
	{
		*out = &std::cout;
		return;
	}

	std::string outFile(replacement);
	if (replacement.empty())
	{
		// get prefix
		size_t dotIndex = originalFile.rfind('.');
		if (dotIndex == std::string::npos)
			dotIndex = originalFile.size();

		outFile = std::string(originalFile, 0, dotIndex);
		outFile += extension;
	}

	*out = new std::ofstream(outFile);
	freeMe = true;
}

int doWork(const Settings &settings)
{
	// open file
	FILE *in = std::fopen(settings.fileToCompile.c_str(), "r");
	if (in == NULL)
	{
		std::cerr << "Failed to open file '" << settings.fileToCompile << "'" << std::endl;
		return 1;
	}
	yyin = in;

	// create AST
	Document d;
	int ret = yyparse(&d);
	fclose(in);
	if (ret != 0)
		return 2;
	d.createBlocks();

	// print ast
	if (settings.astTree)
	{
		std::ostream *os;
		bool freeMe = false;
		parseFileName(settings.fileToCompile, settings.astOutFile, ".dot", &os, freeMe);
		if (os == NULL)
		{
			std::cerr << "Invalid ast out file" << std::endl;
			return 3;
		}

		GraphPrinter printer(*os);
		printer.printGraph(&d);
		*os << std::endl;

		if (freeMe)
			delete os;
	}

	// TODO optimise

	if (settings.compile)
	{
		std::ostream *os;
		bool freeMe = false;
		parseFileName(settings.fileToCompile, settings.compileOutFile, ".o", &os, freeMe);
		if (os == NULL)
		{
			std::cerr << "Invalid compile out file" << std::endl;
			return 4;
		}

		// TODO compile me

		if (freeMe)
			delete os;
	}

	return 0;
}
