#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "string.h"
#include "MQTTClient.h"

#define ADDRESS     "tcp://broker.emqx.io:1883"
#define CLIENTID    "emqx_test"
#define TOPIC       "testtopic/1"
#define QOS         1
#define TIMEOUT     10000L

//declare functions
void clearBoard();
void printBoard();
int checkTie();
void playerMove1();
void playerMove2();
void computerMove();
char checkWinner();
void printExBoard();


//declare variables
char board[9];
char player1 = 'X';
char player2 = 'O';
char computer = 'O';
char payload = ' ';

int messageArrived(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
    printf("Message arrived\n");
    printf("   topic: %s\n", topicName);
    printf("   message: .*s\n", message.payloadlen, message.payload);

    payload = message;
    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

int main()
{
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_message pubmsg = MQTTClient_message_initializer;
    MQTTClient_deliveryToken token;
    int rc;

    MQTTClient_create(&client, ADDRESS, CLIENTID,
        MQTTCLIENT_PERSISTENCE_NONE, NULL);
  
    // MQTT Connection parameters
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }

    int loop = 1;

    do
    {
        char winner = ' ';
        int mode = 0;

            printf("Player 1 is X and Computer is O.\n");

            printExBoard();

            clearBoard();

            while (winner == ' ' && checkTie() != 0)
            {
                printBoard();
                printf("Player 1's Turn (X)\n");
                playerMove1();
                winner = checkWinner();
                if (winner != ' ' || checkTie() == 0)
                {
                    break;
                }
                printBoard();
                printf("Computer's Turn (O)\n");
                computerMove();
                winner = checkWinner();
                if (winner != ' ' || checkTie() == 0)
                {
                    break;
                }
            }

            printBoard();

            if (winner == player1)
            {
                printf("You Win!");
            }
            else if (winner == computer)
            {
                printf("You lose!");
            }
            else
            {
                printf("It is a Tie!");
            }

            loop = 2;

            if (loop == 2)
            {
                printf("Thank you for playing!");
                break;
            }

    } while (loop == 1);

   return 0;
}

void clearBoard()
{
    for (int i = 0; i < 9; i++)
    {
        board[i] = ' ';
    }
}

void printBoard()
{
    printf(" %c | %c | %c ", board[0], board[1], board[2]);
    printf("\n---|---|---\n");
    printf(" %c | %c | %c ", board[3], board[4], board[5]);
    printf("\n---|---|---\n");
    printf(" %c | %c | %c ", board[6], board[7], board[8]);
    printf("\n");
}

void printExBoard()
{
    printf("Each space is represented by the following numbers: \n");
    printf(" 1 | 2 | 3 ");
    printf("\n---|---|---\n");
    printf(" 4 | 5 | 6 ");
    printf("\n---|---|---\n");
    printf(" 7 | 8 | 9 ");
    printf("\n");
    printf("\n");
}

int checkTie()
{
    int spacesFree = 9;

    for (int i = 0; i < 9; i++)
    {
        if (board[i] != ' ')
        {
            spacesFree--;
        }
    }
    return spacesFree;
}

void playerMove1()
{
    int place1;

    do
    {
        place1 = payload - '0';
        place1--;

        if (board[place1] != ' ')
        {
            printf("Invalid Input\n");
        }
        else
        {
            board[place1] = player1;
            pubmsg.payload = payload;
            pubmsg.payloadlen = strlen(payload);
            pubmsg.qos = QOS;
            pubmsg.retained = 0;
            MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
            printf("Waiting for up to %d seconds for publication of %s\n"
                    "on topic %s for client with ClientID: %s\n",
                    (int)(TIMEOUT/1000), PAYLOAD, TOPIC, CLIENTID);
            rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
            printf("Message with delivery token %d delivered\n", token);
            break;
        }
    } while (board[place1] != ' ');
    
}

void playerMove2()
{
    int place2;

    do
    {
        printf("Enter your move(1-9): ");
        scanf("%d", &place2);
        place2--;

        if (board[place2] != ' ')
        {
            printf("Invalid Input\n");
        }
        else
        {
            board[place2] = player2;
            break;
        }
    } while (board[place2] != ' ');
    
}

char checkWinner()
{
    //rows
    if (board[0] == board[1] && board[0] == board[2])
    {
        return board[0];
    }
    if (board[3] == board[4] && board[3] == board[5])
    {
        return board[3];
    }
    if (board[6] == board[7] && board[6] == board[8])
    {
        return board[6];
    }

    //columns
    if (board[0] == board[3] && board[0] == board[6])
    {
        return board[0];
    }
    if (board[1] == board[4] && board[1] == board[7])
    {
        return board[1];
    }
    if (board[2] == board[5] && board[2] == board[8])
    {
        return board[2];
    }

    //diagonals
    if (board[0] == board[4] && board[0] == board[8])
    {
        return board[0];
    }
    if (board[2] == board[4] && board[2] == board[6])
    {
        return board[2];
    }

    return ' ';
}

void computerMove()
{
    srand(time(0));
    int place3;

    do
    {
        place3 = rand() % 9;
    } while (board[place3] != ' ');
        
    board[place3] = computer;
    computer++;
    pubmsg.payload = computer;
    pubmsg.payloadlen = strlen(computer);
    pubmsg.qos = QOS;
    pubmsg.retained = 0;
    MQTTClient_publishMessage(client, TOPIC, &pubmsg, &token);
    printf("Waiting for up to %d seconds for publication of %s\n"
            "on topic %s for client with ClientID: %s\n",
            (int)(TIMEOUT/1000), PAYLOAD, TOPIC, CLIENTID);
    rc = MQTTClient_waitForCompletion(client, token, TIMEOUT);
    printf("Message with delivery token %d delivered\n", token);
}
