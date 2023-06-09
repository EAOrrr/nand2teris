#include"tool.h"
using std::cout;
using std::endl;    using std::cerr;
using std::string;  using std::map; 
int main(int argc, char**argv){
    if(argc != 2) {
            cerr << "2 arguments required" << endl;
            return EXIT_FAILURE;
        }
        std::ifstream iasm(argv[1]);
        if(!iasm.is_open()){
            cerr << "Failed to open " << argv[1] << endl;
            return EXIT_FAILURE;
        }
        std::ofstream ohack(outname(argv[1]));
        SYMBOL_TABLE SYM_B;
    if(false){
        /*debug*/

        /*// for outname
        cout << outname("max.asm") << endl;
        cout << outname("jey") <<endl;
        cout << outname("me.asm") << endl;
        */

    //    // for getline and getmoreline and removeBlank
    //    while(hasMoreLine(iasm)){
    //         string line;
    //         std::getline(iasm, line);
    //         removeBlank(line);
    //         if (line.empty()) continue;
    //         cout << line << endl;
            // if(isSymb(line))
            //     cout << " symb ";
            // else if(isAinstruction(line))
            //     cout << " Ainst ";
            // else if(isCinstruction(line))
            //     cout << "Cinst";   
            // cout << endl << line << endl;
    //    }

        //     // for parse instA
        //     parseAInst("@34", ohack, SYM_B);
        //     parseAInst("@i", ohack, SYM_B);
        //    parseAInst("@i", ohack, SYM_B);
        //     parseAInst("@sum", ohack, SYM_B);
        //     parseAInst("@KBD", ohack, SYM_B);
        // //     SYM_B.print();

        //     // for parse symbol
        //     parseSymb("(LOOP)", 56, SYM_B);
        //     parseSymb("(END)", 39, SYM_B);
        //     parseAInst("@END", ohack, SYM_B);
        //     SYM_B.print();

        //for parse instC
        parseCInst("M=D", ohack);
        parseCInst("D=M;JEQ", ohack);
        parseCInst("0;JMP", ohack);
        parseCInst("0", ohack);
    }
    else{
        /*main*/
        
        /*first pass*/
        int row = 0;
        while(hasMoreLine(iasm)){
            string line;
            std::getline(iasm, line);
            removeBlank(line);
            if(isSymb(line)){
                parseSymb(line, row, SYM_B);
            }
            else if(isAinstruction(line) || isCinstruction(line)){
                row++;
            }
        }
        /*second pass*/
        iasm.close();
        iasm.open(argv[1]);
        while(hasMoreLine(iasm)){
            string line;
            std::getline(iasm, line);
            removeBlank(line);
            if(isAinstruction(line)){
                try{
                    parseAInst(line, ohack, SYM_B);
                }
                catch(std::logic_error const& ex){
                    // cout << line << endl;
                }
            }
            // SYM_B.print();
            if(isCinstruction(line)){
                try{
                    parseCInst(line, ohack);
                }
                catch(std::logic_error const& ex){
                    cout << line << endl <<  ex.what() << endl;
                }
            }
        }
    }
    // iasm.close();
    // ohack.close();
    return EXIT_SUCCESS;
}