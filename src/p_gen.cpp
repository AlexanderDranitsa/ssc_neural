#include "p_gen.h"

P_GEN::P_GEN(sc_module_name nm)
    :sc_module(nm),
    clk_i("clk_i"),
    request("request"),
    req_write("req_write"),
    done("done")
{
    SC_METHOD(gen);
    sensitive << clk_i.pos();
}

P_GEN::~P_GEN()
{
}

void P_GEN::gen()
{
    req_write.write(0);
    done.write(0);
    if (request.read()){
        cout<<"GOT REQUEST" << endl;
        for (int i = 0; i < 49; i++){
                fig[i] = 0;
            }
            for (int i = 0; i < 3; i++){
                ref[i] = 0;
            }

        int rnd = rand();
        cout << "  RND: " << std::dec << rnd << endl;
        if ((rnd % 9) <= 3) {
            for (int i = 0; i < 49; i++){
                fig[i] = m_round[i];
            }
            ref[2] = 1;
        } else if ((rnd % 9) <= 6) {
            for (int i = 0; i < 49; i++){
                fig[i] = m_triangle[i];
            }
            ref[1] = 1;
        } else {
            for (int i = 0; i < 49; i++){
                fig[i] = m_square[i];
            }
            ref[0]= 1;
        }

        int i = HARASS_LVL;
        while(i){
            int rnd= rand() % 49;
            if (fig[rnd] == 0){
                fig[rnd] = 1;
                int flag = 1;
                while(flag){
                    int rnd= rand() % 49;
                    if (fig[rnd] == 1){
                        fig[rnd] = 0;
                        flag = 0;
                    }
                }
                i--;
            }
        }
        counter = 0;
        flag = 1;
    }
    if (flag){
        shared_a = INPUT+counter;
        shared_d = fig[counter];
        cout << "ADDR: " << shared_a << " DATA: " << shared_d << endl;
        req_write.write(1);
        next_trigger();
        counter++;
        if (counter == 49){
            flag = 0;
            counter = 0;
            cout << "HERE1" << endl;
            done.write(1);
        }
    }
}