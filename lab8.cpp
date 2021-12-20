#include <mpi.h>
// #include <stdio.h>
#include <iostream>
#include <memory>
#include <vector>
using namespace std;

struct Ball
{
    Ball(int size)
    {
        data.resize(size);
        data[0] = 0;
    }

    void resize(int size)
    {
        data.resize(size);
        data[0] = 0;
    }
    auto Size() { return data.size(); }
    auto &operator[](size_t i) { return data[i]; }

private:
    vector<int> data;
};

struct IPlayer
{
    virtual void Play() = 0;
};

struct Idle : IPlayer
{
    void Play() override{};
};

struct Player : IPlayer
{
    Player(Ball &_ball, bool _iAmGameStarter) : ball(_ball), iAmGameStarter(_iAmGameStarter)
    {

        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        // At least 2 processors needed
        if (size < 2)
        {
            throw;
        }

        target = (rank + 1) % 2;
    };

    void SendBall()
    {
        MPI_Send(&ball[0], ball.Size(), MPI_INT, target, 0, MPI_COMM_WORLD);
    }
    void RecvBall()
    {
        MPI_Recv(&ball[0], ball.Size(), MPI_INT, target, 0, MPI_COMM_WORLD, &stat);
    }
    void Play() override
    {
        if (iAmGameStarter)
            SendBall();
        RecvBall();
        ball[0]++;
        // cout << "Rank :" << rank << " has the ball, No of throws: " << ball[0] << endl;
        if (!iAmGameStarter)
            SendBall();
    }

private:
    Ball &ball;
    int rank;
    int size;
    int target;
    bool iAmGameStarter;
    MPI_Request req;
    MPI_Status stat;
};

auto MakeUniquePlayer(Ball &ball, int rank)
{
    // Only rank 0 and 1 play the game
    if (rank == 0)
    {
        // Rank 0 starts the game
        return unique_ptr<IPlayer>(new Player(ball, true));
    }
    else if (rank == 1)
    {
        return unique_ptr<IPlayer>(new Player(ball, false));
    }
    else
    {
        return unique_ptr<IPlayer>(new Idle());
    }
}

int main(int argc, char *argv[])
{

    MPI_Init(&argc, &argv);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Customize here
    int ballSize = 1;
    int resizeIterations = 6;
    int mainIterations = 16;

    Ball ball(ballSize);

    auto player = MakeUniquePlayer(ball, rank);

    // Warm up players before the game
    for (int size_i = 0; size_i < resizeIterations; size_i++)
    {
        ballSize *= 10;
        ball.resize(ballSize);
        double start = MPI_Wtime();
        for (int i = 0; i < mainIterations; i++)
            player->Play();
        double end = MPI_Wtime();

        auto elapsedTime = end - start;
        // each iteration has 2 transfers: send and receive
        // auto transferTime = elapsedTime / mainIterations;
        auto ballSizeInMegaByte = ballSize * 4.0 /* byte */ / 1000000;

        if (rank == 0)
        {
            cout << "#" << size_i << endl;
            cout << "Size (MB): " << ballSizeInMegaByte << endl;
            cout << "Transfer time (Sec): " << elapsedTime << endl;
            cout << "Bandwidth (MB/s): " << 2 * ballSizeInMegaByte * mainIterations / elapsedTime << endl;
        }
    }
    MPI_Finalize();
}