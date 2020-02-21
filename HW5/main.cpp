#include <iostream>
#include <fstream>
#include <iomanip>
#include "mpi.h"

#define MASTER 0

int main(int argc , char *argv[])
{
    int maxTime;
    double maxThrust;
    double spaceShips2D[8][7];
    double spaceShips[72];

    int numtasks, rank, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Get_processor_name(hostname, &len);


    srand(rank);
    if(rank == 0)
    {
        //loadData();
        std::ifstream f;
        f.open("in2.dat");
        f >> maxTime;
        f >> maxThrust;
        for(int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 7; j++)
            {
                f >> spaceShips2D[i][j];
            }
        }
        std::cout <<std::endl;
        std::cout <<std::endl;
        int count = 0;
        for(int i = 0; i < 8; i++) {
            for (int j = 2; j < 9; j++) {
                spaceShips[i*9 + j] = spaceShips2D[i][j-2];
                count++;
            }
            spaceShips[i*9] = i;
            spaceShips[i*9 + 1] = 1;
        }
//        for(int i = 0; i < 8; i++){
//            for(int j = 0; j < 9; j++){
//                std::cout << " " << spaceShips[i*9 + j];
//            }
//            std::cout <<std::endl;
//        }
//        std::cout <<std::endl;
//        std::cout <<std::endl;
    }
//    std::cout << maxTime << std::endl;



    MPI_Bcast(&spaceShips[0], 72, MPI_DOUBLE, 0, MPI_COMM_WORLD);


    for(int time=0; time < 5000; ++time)
    {
        if(rank==0)
        {
//            std::cout << time << std::endl;
            double sendBuf[9];
            for(int i = 0; i < 9; i++){
                sendBuf[i] = spaceShips[rank*9 + i];
            }
            //calculate directional velocities
            double posX = sendBuf[2];
            double posY = sendBuf[3];
            double posZ = sendBuf[4];

            double dirX = sendBuf[6];
            double dirY = sendBuf[7];
            double dirZ = sendBuf[8];

            double initialSpeed = sendBuf[5];

            double newX = posX + initialSpeed*dirX;
            double newY = posY + initialSpeed*dirY;
            double newZ = posZ + initialSpeed*dirZ;

            sendBuf[2] = newX;
            sendBuf[3] = newY;
            sendBuf[4] = newZ;

            MPI_Allgather(sendBuf,9,MPI_DOUBLE,spaceShips,9,MPI_DOUBLE,MPI_COMM_WORLD); // spaceShips array doesnt update

            for(int i = 1; i < 8; i++)
            {
                std::cout << std::fixed << std::setprecision(0) << spaceShips[i*9] << ", " << spaceShips[i*9 + 1];
                std::cout << std::scientific << std::setprecision(6) << ", " << spaceShips[i*9 + 2] <<
                ", " << spaceShips[i*9 + 3] << ", " << spaceShips[i*9 + 4] << ", " << spaceShips[i*9 + 6] <<
                    ", " << spaceShips[i*9 + 7] << ", " << spaceShips[i*9 + 8] << std::endl;

            }
        }
        else
        {
            double sendBuf[9];
            for(int i = 0; i < 9; i++){
                sendBuf[i] = spaceShips[rank*9 + i];
            }

            //calculate directional velocities
            double posX = sendBuf[2];
            double posY = sendBuf[3];
            double posZ = sendBuf[4];

            double dirX = sendBuf[6];
            double dirY = sendBuf[7];
            double dirZ = sendBuf[8];

            double initialSpeed = sendBuf[5];

            double newX = posX + initialSpeed*dirX;
            double newY = posY + initialSpeed*dirY;
            double newZ = posZ + initialSpeed*dirZ;

            sendBuf[2] = newX;
            sendBuf[3] = newY;
            sendBuf[4] = newZ;

            MPI_Allgather(sendBuf,9,MPI_DOUBLE,spaceShips,9,MPI_DOUBLE,MPI_COMM_WORLD);
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



//    if(rank == 0) {
//        printf("END OF CODE SPACESHIPS RANK: %d\n", rank);
//        for(int i = 0; i < 8; i++){
//            for(int j = 0; j < 9; j++){
//                std::cout << " " << spaceShips[i*9 + j];
//            }
//            std::cout <<std::endl;
//        }
//        std::cout <<std::endl;
//        std::cout <<std::endl;
//    }
    MPI_Finalize();
    return 0;
}