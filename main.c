#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <stdlib.h>

 /*É NECESSARIO TROCAR O CAMINHO DO ARQUIVO NA FUNÇÃO GAME OVER E NA FUNÇÃO SCORE!!!*/

typedef struct Item
{

  int id;
  int type[2];					/*respectivamente, o tipo  de item e o quanto ele recupera */
  char name[25];
  char description[200];
  float chance;
  int damage;
  int throwable[2];				/*respectivamente, jogavel sim ou não. o quanto de dano causa */
  int craftable[2];				/*respectivamente, craftavel sim ou não, id do craft */
  int durability[2];			/*Se tem durabilidade sim ou não, quanto de durabilidade tem (max 10) */

} Item;

typedef struct Enemy
{

  char name[25];
  int status[2]; /*respectivamente, vida e força*/
  int max_life;
  int chance;					/*chance de aparecer */
  Item in_hand;
  int body;
  char parts[4][25];

  int first_encounter[2];		/*respectivamente, se já encontrou ou não e quanto de sanidade irá perder no primeiro encontro */

} Enemy;

typedef struct Pool
{

  Item drops[4];
  Enemy encounter;

} Pool;

typedef struct Area
{

  char name[25];
  int risk;
  Pool loot;

} Area;

typedef struct Shack
{

  char name[25];
  int bed_window[2];			/* respectivamente, barricada sim ou não, com com armadilha sim ou não */
  int living_window[2];			/* respectivamente, barricada sim ou não, com com armadilha sim ou não */
  int kitchen_window[2];		/* respectivamente, barricada sim ou não, com com armadilha sim ou não */

  int living_door[3];			/* respectivamente, barricada sim ou não, com armadilha sim ou não, quebrada sim ou não */
  int back_door[3];				/* respectivamente, barricada sim ou não, com armadilha sim ou não, quebrada sim ou não */

  int lamp_1[2];				/*respectivamente, ligada sim ou não, quebrada sim ou não */
  int lamp_2[2];				/*respectivamente, ligada sim ou não, quebrada sim ou não */
  Item chest[25];
  int hour;
  int generator;

} Shack;

typedef struct Player
{

  char name[25];
  int status[6];				/*são respectivamente vida, força, energia, sanidade, notoriedade e fome */
  int max_life, max_hunger, max_energy;
  Item in_hand;
  Item bag[6];
  int days;

} Player;

/*função que retorna um número aletório baseado no range dado*/
int coin(int flips) {
    return (rand() % (flips + 1));
}
/*Função que limpa a tela*/
void clear()
{
  system("cls");
}
/*função que manda um print customizado.*/
void print(char text[1]) {
	system("cls");
    printf("\n %s \n", text);
    getchar();
    getchar();
	system("cls");
}

/*função que procura item no inventáio e retorna o index*/
int search_item (Item item, Player player)
{

  int i;
  for (i = 0; i < 6; i++)
	{

	  if (player.bag[i].id == item.id)
		{

		  return i;

		}

	}

  return -1;

}

/*função que dropa um item da mochila do jogador (talvez retornar o item)*/
int drop_item_from_bag (int index, Player * player, Item nada)
{

  if (index < 0 || index > 4) {
      print("É, não vai funcionar.");
      return -1;
  }

  if (player->bag[index].id == nada.id)
	{
	  return -1;
	}

  player->bag[index] = nada;
  return 1;
}

/*função que dropa um item da mão do jogador*/
int drop_item_from_hand (Player * player, Item nada)
{

  if (player->in_hand.id == nada.id)
	{

	  return -1;
	}

  player->in_hand = nada;
  return 1;

}



/*função que lista os itens do jogador*/
void items_list (Player player, Item nada)
{

  int i;
  printf ("MOCHILA DO(A) %s:\n", player.name);
  for (i = 0; i < 5; i++)
	{

	  if (player.bag[i].id == nada.id)
		{

		  printf ("%d -> nada \n", i + 1);
		}

	  else
		{

		  printf ("%d -> %s \n", i + 1, player.bag[i].name);

		}

	}

  printf ("NA MÃO: %s \n", player.in_hand.name);

}

/*função que guarda um item da mão no primeiro espaço vazio da mochila.*/
int hand_to_bag (Player* player, Item nada)
{
  if (player->in_hand.id == nada.id)
	{
	  print("Minhas mãos estam vazias...");
	  return -1;
	}

  int bagIndex = search_item (nada, *player);

  if (bagIndex == -1)
	{
	  print("Minha mochila está cheia, droga!");
	  return -1;
	}

  player->bag[bagIndex] = player->in_hand;
  player->in_hand = nada;
  print("Agora minhas mãos estão livres.");
  return 1;
}

/*função que passa um item da mochila para mão.*/
int bag_to_hand (int index, Player* player, Item nada)
{

  if (index < 0 || index > 4) {
      print("Não vai funcionar..");
      return -1;
  }

  if (player->bag[index].id == nada.id)
	{
	  print("Acho que eu ainda preciso respeitar as regras da fisica...");
	  return -1;
	}

  if (player->in_hand.id != nada.id)
	{
	  // mudar aqui (mudando)
	  Item aux = player->in_hand;
	  player->in_hand = player->bag[index];
	  player->bag[index] = aux;
	  return 1;

	}


  player->in_hand = player->bag[index];
  player->bag[index] = nada;
  return 1;
}

/*função que lista os itens do baú*/
void chest_list (Item chest[])
{
  int i;
  for (i = 0; i < 25; i++)

	printf ("-%s \n", chest[i].name);

}

/*função que percorre o baú até achar um espaço vazio*/
int chest_free_space (Shack shack, Item nada) {

    int a;
    do {

        if (shack.chest[a].id == nada.id) {
            return a;
        }

        a++;


    }while(0 == 0);

    return -1;
}
/*função que passa um item pro bau diretamente da mão.*/
int hand_to_chest(Shack* shack, Player* player, Item nada) {
    if (player->in_hand.id != nada.id) {
        shack->chest[chest_free_space(*shack, nada)] = player->in_hand;
        drop_item_from_hand(player, nada);
        return 1;
    }

    print("Minhas mãos estão vazias...");
    return -1;
}

/*função que passa um item da mochila para o baú*/
int bag_to_chest (int index, Shack* shack, Player* player, Item nada)
{

  if (index < 0 || index > 4)
	{
	  print("Isso não vai funcionar...");
	  return -1;
	}

  if (player->bag[index].id == nada.id)
	{
	  print("Tá vazio!");
	  return -1;
	}

  int chestIndex = chest_free_space(*shack, nada);

  /*Esse retorno C) mais voltado pro lado do dev */
  if (chestIndex == -1)
	{
	  print("Cacete, o baú ta cheio.");
	  return -1;
	}

  shack->chest[chestIndex] = player->bag[index];
  /*função que dropa um item da mochila*/
  if (drop_item_from_bag (index, player, nada) == -1)
	{
	  printf ("Erro ao remover item da mochila!\n");
	  return -1;
	}

  return 1;
}
/*função que passa item do baú para mochila.*/
int chest_to_bag (int index, Shack* shack, Player* player, Item nada) {
    if (index < 0 || index > 24) {
        print("Esse baú não é tão grande quanto eu queria que fosse.");
        return -1;
    }

    if (shack->chest[index].id == nada.id) {
        print("Aqui não tem nada...");
        return -1;
    }

    int bagIndex = search_item(nada, *player);

    if (bagIndex == -1 && player->in_hand.id == nada.id) {
        player->in_hand = shack->chest[index];
        shack->chest[index] = nada;
        return 1;
    }

    player->bag[bagIndex] = shack->chest[index];
    shack->chest[index] = nada;
    return 1;
}
/*função que quebra um item na mão caso a durabilidade chegue a zero*/
void durability(Player* player, Item nada) {
    if (player->in_hand.durability[0] == 0) {
        player->in_hand.durability[1]  --;
    }

    if (player->in_hand.durability[1] <= 0 && player->in_hand.id != nada.id) {
        print("Quebrou!");
        drop_item_from_hand(player, nada);

    }
}
/*função que controla alguns status do jogador.*/
void balance(Player* player) {
    if (player->max_life < player->status[0]) {
        player->status[0] = player->max_life;
    }

    if (player->max_energy < player->status[2]) {
        player->status[2] = player->max_energy;
    }

    if (player->max_hunger < player->status[5]) {
        player->status[5] = player->max_hunger;
    }

}

/*função que usa um item da mão do jogador.*/
int use_item_in_hand(Player* player, Item nada) {

    if (player->in_hand.type[0] == -1) {
        print("Não vai dar pra usar isso.");
        return -1;
    }

    Item item = player->in_hand;
    player->status[item.type[0]] += item.type[1];
    drop_item_from_hand(player, nada);
    balance(player);
	system("cls");
    printf("\nAcho que %s vai prestar por agora.\n", item.name);
    getchar();
    getchar();
	system("cls");
    return 1;
}
/*função que usa um item da mochila do jogador.*/
int use_item(int index, Player* player, Item nada) {
    if (index < 0 || index > 5) {
        print("Eu não sei o que eu estou tentando fazer, mas provavelmente não vai funcionar...");
        return -1;
    }

    if (player->bag[index].type[0] == -1) {
        print("Não vai dar pra usar isso.");
        return -1;
    }

    Item item = player->bag[index];
    player->status[item.type[0]] += item.type[1];
    drop_item_from_bag(index, player, nada);
    balance(player);
	system("cls");
    printf("\nAcho que %s vai prestar por agora.\n", item.name);
    getchar();
    getchar();
	system("cls");
    return 1;
}

/*função que adciona gasolina ao gerador*/
int generator_fuel(Player* player, Shack* shack, Item nada, Item gasolina) {
    int bagIndex = search_item(gasolina, *player);

    if (bagIndex == -1 && player->in_hand.id == gasolina.id) {
        shack->generator += 25;
        drop_item_from_hand(player, nada);
        return 1;
    }

    if (bagIndex == -1 && player->in_hand.id != gasolina.id) {
        print("Eu não tenho gasolina.");
        return -1;
    }

    shack->generator += 25;
    player->bag[bagIndex] = nada;
}
/*função que controla e mostra a tela de game over, ela também salva o ultimo score (dias sobrevividos)*/
void game_over(Player* player) {
	if (player->status[0] <= 0) {
		FILE* f = fopen("C:\\Users\\Win\\CLionProjects\\demo_matagal\\score.txt", "w");
		if (f == NULL) {
			printf("Erro ao abrir o arquivo\n");
			exit(1);
		}

		int resultado = fprintf(f, "%d \n", player->days);
		if (resultado < 0) {
			printf("Erro ao escrever no arquivo\n");
			exit(1);  // Encerra o programa em caso de erro
		}

		fclose(f);
		print("Acabou, você escuta seu oração parar aos poucos enquanto sua visão fica turva, mas...\n");
		exit(1);
	}
}

/*função que printa o ultimo score do jogador*/
void score() {
	FILE *f;
	char linha[1024];

	f = fopen("C:\\Users\\Win\\CLionProjects\\demo_matagal\\score.txt", "r");
	if (f == NULL) {
		printf("Erro ao abrir o arquivo.\n");
		exit(1);
	}

	while (fgets(linha, sizeof(linha), f) != NULL) {
		printf("O fortuna do seu ultimo ciclo durou %s dias", linha);
	}

	fclose(f);
}

/*função que define encontros*/ /*NÃO ESQUECER DE TRABALHAR A DURABILIDADE DO ITEM NA MÃO*/
int encounter(Player* player, Enemy enemy, Item nada) {
    print("Merda, me acharam!");
    if (coin(1) == 0) {
        print("Você sente seu coração acelerar, suas pernas fraquejam enquanto sua mente se esvazia de pensamentos coerentes.");
        player->status[3] --;
    }

    do {
        int c;
        clear();
        printf("                 MdJcczvzYYXJcj>                  \n"
        "               @@W#ahpk0zQzvc00Qx)                \n"
        "              @@@Bb##azkdQYkJzndJCp               \n"
        "            WW@@@$BaWahpphbZYvYbXtW              \n"
        "            @$@@@@@@$$*aaadapZJYcrpm#             \n"
        "            M@@@@@@WB$@WW#oQhaMdOnnxz#            \n"
        "         oM@@@BBBWWWB#W*hbZQpOZakddQnn)YQ         \n"
        "       $B#*$@@@@@@@$$BWMW$@WW@@oW@@@@*ddxxOB      \n"
        "     @@@@@$$$@@@@@$hpzc@$hBOW@$@$$@@@@@@@@@Bk     \n"
        "     @$$@@@@@@@@@@$kWaYmccc]pJp#W#@@$@$B$Mak      \n"
        "        @@B@$@@@@#W$*d0Cd0]vrccddd*Bb             \n"
        "            WW@@@$Boobmhr_QXQxaa#@@ok             \n"
        "            WB@@@@bModdOxapxd0rkbh$d              \n"
        "             M$@Bh*WahhdphzOYQkdWBab              \n"
        "              *a@@@Mkbo*QdkXpOMOBMB               \n"
        "               Mo$$$BB$o*CZQhd*W@@h@              \n"
        "              MB@@BWo#aMdwwoh#W$@@@bp             \n"
        "            ##@@@@@@@@@W*ahh@B$@MW@@Zj@           \n"
        "       h*oMM@@@@@@@@@@$BW$@$*k##W@$@@0oQmZ@       \n"
        "   hk#MMMMWB$@@@@@@@@B#Mo#B#daob#@@@W#pOoZzcCC*@  \n"
        "oMWM###MWWB@@@@@@@@$@$BWWWo*kadhWo#WM*aJO$WhkapX0d\n"
        "MM##MMMMWW$@@@@@@@@@$W$$aaOodho$*MBW*hphbd@$WawCC0\n"
        "#MMMMMMWWB$@@@@@@@@@$M#okdab*WBaoBMW*Zo#$@@@#*kwJJ\n"
        "MMMMMMWWWWBB@@@@@@@@@@M*ok*a$WoBB@@okdaQaQ0@@$*#d0\n"
        "MMMMMMWWWWW$$@@@@@@@@@@BM#B$B#BBB$kaawdbZmzJb$$*bm        \n");
        printf("+--------------------------------------------------+\n");
        printf("Vida: %d \n", player->status[0]);
        printf("Força: %d \n", player->status[1]);
        printf("NA MÃO: %s \n", player->in_hand.name);
        printf("\n %s -> VIDA: %d \n", enemy.name, enemy.status[0]);
        printf("\n[1] Bater \n");
        printf("[2] Defender \n");
        printf("[3] Gritar \n");
        printf("\nO que fazer?\n");
        printf("-> ");
        scanf("%d", &c);

        if (c == 1) {
            printf("\nVocê se prepara desesperadamente para atacar.\n");
            getchar();
            getchar();

            if (coin(1) == 0) {
                printf("%s assume uma postura defensiva.\n", enemy.name);
                getchar();
                int damage = (player->status[1] + coin(player->in_hand.damage)) - coin(enemy.status[1]);
                if (damage <= 0) {
                    printf("Deu tudo errado.\n");
                    getchar();
                }

                else {
                    enemy.status[0] -= damage;
                    durability(player, nada);
                }
            }

            else {
                printf("%s assume uma postura ofensiva.\n", enemy.name);
                getchar();
                int playerDMG = player->status[1] + coin(player->in_hand.damage);
                int enemyDMG = enemy.status[1] + coin(enemy.in_hand.damage);

                enemy.status[0] -= playerDMG;
                player->status[0] -= enemyDMG;
            } /*continuar fazendo o defender (feito)*/
        }

        if (c == 2) {
            printf("\nVocê bloqueia a maioria dos orgãos vitais.\n");
            getchar();
            getchar();
            printf("%s assume uma postura ofensiva.\n", enemy.name);
            getchar();
            int defense = coin(player->status[1]);
            int enemyDMG = enemy.status[1] + coin(enemy.in_hand.damage) - defense;
            if (enemyDMG <= 0) {
                printf("Você sai ileso.\n");
                getchar();
            }

            else {
                player->status[0] -= enemyDMG;
            }
        }

        if (c == 3) {
            printf("\nDesesperado, você grita afim de espantar a ameaça eminente.\n");
            getchar();

            if (coin(1) == 0) {
                printf("O(A) %s se vê intimiado pelos seus gritos guturais, ele foge, mas você sente que chamou mais atenção do que deveria.\n", enemy.name);
                getchar();
                player->status[4] ++;
                break;
            }

            else {
                printf("Você grita, porém isso só parece ter deixado a criatura mais agitada e irritada\n");
                getchar();
                printf("Ela assume uma postura extremamente ofensiva.\n");
                getchar();
                int enemyDMG = enemy.status[1] * 2;
                player->status[0] -= enemyDMG;
            }
        }

    	system("cls");


    }while(enemy.status[0] > 0 && player->status[0] > 0);
	game_over(player);
}


/*função que leva o player para explorar as areas disponiveis*/
void explore(Player* player, Area area, Item nada) {

	system("cls");
    printf("\nVocê Saiu para explorar %s \n", area.name);
    getchar();
	system("cls");
    int i = 1;

    do {

        if (player->status[2] <= 0) {
            print("Tô cansado demais pra isso...");
            player->status[2] = 0;
            break;
    }

        int ant = player->status[4] + area.risk + area.loot.encounter.chance;
        int prox = coin((player->status[4] * 2) + area.risk + area.loot.encounter.chance);
        print("Espero achar algo de bom nessa imundisse...");

        if (prox < ant) {
            print("O que é aquilo? Melhor eu ir ver, talvez seja algo útil.");

            if (coin(1) == 0) {
                Item loot = area.loot.drops[coin(3)];
                int bagIndex = search_item(nada, *player);

                if (bagIndex == -1) {
                    print("Merda, tô sem espaço na mochila.");
                }

                else {
                	system("cls");
                    printf("\nÓtimo, encontrei %s \n", loot.name);
                    getchar();
                	system("cls");
                    player->bag[bagIndex] = loot;
                }

            }

            else {
                print("Merda, não é nada...");
            }
        }

        else {
            print("Eu estou sendo seguido.");

            if (coin(1) == 0) {
                encounter(player, area.loot.encounter, nada);
            }

            else {
                print("Acho que consegui despistar.");
            }
        }

        if (player->status[2] <= 0) {

            print("Eu estou cansado demais para continuar.");
            i = 0;
        }

        if (coin(1) == 0) {
            player->status[2] --;
        }


        printf("sair?[0]");
        scanf("%d", &i);
    	system("cls");



    }while(i != 0 && player->status[0] > 0);

    print("Eu sinto que estou chamando atenção indesejada.");
    player->status[4] ++;

}
/*função que controla os eventos noturnos*/
void night(Player* player, Shack* shack, Enemy enemy_1, Enemy enemy_2, Item nada) {
    int risk = 8;
    shack->hour = 0;
    int fuel = coin(25);
    print("A noite chegou, ascenda as luzes, fique em casa e sobreviva.");

    if (shack->generator >= fuel) {
        print("Você liga o antigo gerador que garante um local bem iluminado.");
        shack->generator -= fuel;
    }

    else {
        print("O gerador não liga.");
        risk += coin(7);
    }

    if (shack->living_window[0] == 0) {
        print("Você fica meio histérico olhando para janela.");
        risk ++;
    }

    if (shack->kitchen_window[0] == 0) {
        print("Barulhos de paços podem ser ouvidos do lado de fora. Você está sendo sondado.");
        risk ++;
    }

    if (shack->living_door[0] == 0) {
        print("Você improvisa colocando uma cadeira na porta.");
        risk++;
    }

    if (shack->living_door[1] == 1) {
        print("Talvez você possa sair vivo dessa, talvez.");
        risk -= 2;
    }



    do {

       game_over(player);

       Enemy enemies[2];
       enemies[0] = enemy_1;
       enemies[1] = enemy_2;
       int chance = coin(player->status[4] + risk);

       if (chance == 0) {
           print("Nada acontece.");
           player->status[2] ++;
           balance(player);
           shack->hour ++;
       }

        if (chance == 1) {
           print("Você escuta o ranger da madeira velha enquanto procura qualquer anormalidade.");
           shack->hour ++;
       }

        if (chance == 2) {
           print("Você reflete e tenta se lembrar como veio parar aqui, porém nada.");
           player->status[0] ++;
           balance(player);
           shack->hour ++;
       }

        if (chance == 3) {
           print("Lobos, você os escuta a distância.");
           player->status[4] --;
           if (player->status[4] < 0) {
               player->status[4] = 0;
           }
           shack->hour ++;
       }

        if (chance == 4) {
           print("A luz das lampadas falham, grunidos podem ser ouvidos agora, não são lobos.");
           shack->hour ++;
       }

        if (chance == 5) {
           print("Você está tremendo, pancadas na porta te fazem correr até ela e segurar com o ombro.");
           player->status[3] --;
           if (player->status[3] < 0) {
               player->status[3] = 0;
           }
           shack->hour ++;
       }

        if (chance == 6) {
           print("A fome aperta.");
           player->status[5] --;
           if (player->status[5] < 0) {
               player->status[5] = 0;
           }
           shack->hour ++;
       }

        if (chance == 7) {
           print("Por que eu não acabo com tudo?");
           player->status[3] --;
           if (player->status[3] < 0) {
               player->status[3] = 0;
           }
           shack->hour ++;
       }

        if (chance == 8) {
           print("Você pisa em um objeto pontudo que deixou cair durante o dia, a ferida dói.");
           player->status[0] --;
           shack->hour ++;
       }

        if (chance == 9) {
           print("Deus, me perdoa por favor.");
           player->status[3] --;
           if (player->status[3] < 0) {
               player->status[3] = 0;
           }
           shack->hour ++;
       }

        if (chance == 10) {
           print("Nada acontece.");
           shack->hour ++;
       }

        if (chance > 10) {
           print("...");
           encounter(player, enemies[coin(1)], nada);
           shack->hour ++;
       }

    }while(shack->hour < 6 && player->status[0] > 0);

    player->status[2] = coin(player->status[5]);
    balance(player);
    player->status[5] --;
    if (player->status[5] < 0) {
               player->status[5] = 0;
           }

    if (player->status[0] > 0) {
        player->status[0] += 5;
        balance(player);
        print("A noite acabou, o Sol começa a raiar.");
    }

    player->status[4] --;
    if (player->status[4] < 0) {
               player->status[4] = 0;
           }

	player->days ++;

}

/*função que cria a copia de um item TALVEZ ALOCAÇÃO?*/
Item copyItem (Item original)
{
  Item copy;

  /*Copie cada propriedade do item original para a copia */
  strcpy (copy.name, original.name);
  strcpy (copy.description, original.description);
  copy.type[0] = original.type[0];
  copy.type[1] = original.type[1];
  copy.chance = original.chance;
  copy.damage = original.damage;
  copy.throwable[0] = original.throwable[0];
  copy.throwable[1] = original.throwable[1];
  copy.craftable[0] = original.craftable[0];
  copy.craftable[1] = original.craftable[1];
  copy.durability[0] = original.durability[0];
  copy.durability[1] = original.durability[1];
  copy.id = original.id;		// ID imutC!vel

  return copy;
}

int
main ()
{
	/*Definindo as structs no main*/
  srand(time(NULL));
  Item graveto;
  strcpy (graveto.name, "graveto");
  strcpy (graveto.description, "um graveto");
  graveto.type[0] = -1;
  graveto.chance = 3;
  graveto.damage = 1;
  graveto.throwable[0], graveto.throwable[1] = 0;
  graveto.craftable[0] = 1;
  graveto.craftable[1] = 1;
  graveto.durability[0], graveto.durability[1] = 0;
  graveto.id = 1;

  Item tabua;
  strcpy (tabua.name, "tabua");
  strcpy (tabua.description, "tabua de madeira");
  tabua.type[0] = -1;
  tabua.chance = 5;
  tabua.damage = 0;
  tabua.throwable[0], tabua.throwable[1] = 0;
  tabua.craftable[0], tabua.craftable[1] = 0;
  tabua.durability[0], tabua.durability[1] = 0;
  tabua.id = 2;

  Item pedra;
  strcpy (pedra.name, "pedra");
  strcpy (pedra.description, "uma pedra");
  pedra.type[0] = -1;
  pedra.chance = 3;
  pedra.damage = 1;
  pedra.throwable[0] = 1;
  pedra.throwable[1] = 2;
  pedra.craftable[0], pedra.craftable[1] = 0;
  pedra.durability[0] = 1;
  pedra.durability[1] = 1;
  pedra.id = 3;

  Item batata;
  strcpy (batata.name, "batata");
  strcpy (batata.description, "uma batata");
  batata.type[0] = 5;
  batata.type[1] = 1;
  batata.chance = 2;
  batata.damage = 1;
  batata.throwable[0] = 1;
  batata.throwable[1] = 1;
  batata.craftable[0], batata.craftable[1] = 0;
  batata.durability[0] = 1;
  batata.durability[1] = 1;
  batata.id = 4;

  Item lanca_ponta_de_madeira;
  strcpy (lanca_ponta_de_madeira.name, "lança de madeira");
  strcpy (lanca_ponta_de_madeira.description, "lança com a ponta de madeira");
  lanca_ponta_de_madeira.type[0] = -1;
  lanca_ponta_de_madeira.chance = 7;
  lanca_ponta_de_madeira.damage = 4;
  lanca_ponta_de_madeira.throwable[0] = 1;
  lanca_ponta_de_madeira.throwable[1] = 4;
  lanca_ponta_de_madeira.craftable[0] = 1;
  lanca_ponta_de_madeira.craftable[1] = 2;
  lanca_ponta_de_madeira.durability[0] = 1;
  lanca_ponta_de_madeira.durability[1] = 5;
  lanca_ponta_de_madeira.id = 5;

  Item prego;
  strcpy (prego.name, "prego");
  strcpy (prego.description, "um prego");
  prego.type[0] = -1;
  prego.chance = 1;
  prego.damage = 0;
  prego.throwable[0], prego.throwable[1] = 0;
  prego.craftable[0], prego.craftable[1] = 0;
  prego.durability[0], prego.durability[1] = 0;
  prego.id = 6;

  Item gasolina;
  strcpy (gasolina.name, "gasolina");
  strcpy (gasolina.description, "um tanque de gasolina");
  gasolina.type[0] = -1;
  gasolina.chance = 2;
  gasolina.damage = 0;
  gasolina.throwable[0], gasolina.throwable[1] = 0;
  gasolina.craftable[0], gasolina.craftable[1] = 0;
  gasolina.durability[0], gasolina.durability[1] = 0;
  gasolina.id = 7;

  Item armadilha_urso;
  strcpy (armadilha_urso.name, "Armadilha de Urso");
  strcpy (armadilha_urso.description, "Armadilha para uso, deve doer.");
  armadilha_urso.type[0] = -1;
  armadilha_urso.chance = 4;
  armadilha_urso.damage = 0;
  armadilha_urso.throwable[0], armadilha_urso.throwable[1] = 0;
  armadilha_urso.craftable[0] = 1;
  armadilha_urso.craftable[1] = 4;
  armadilha_urso.durability[0], armadilha_urso.durability[1] = 0;
  armadilha_urso.id = 8;

  Item nada;
  strcpy (nada.name, "nada");
  strcpy (nada.description, "um monte de nada");
  nada.type[0] = -1;
  nada.chance = 5;
  nada.damage = 0;
  nada.throwable[0], nada.throwable[1] = 0;
  nada.craftable[0], nada.craftable[1] = 0;
  nada.durability[0], nada.durability[1] = 0;
  nada.id = 9;

  Item kit;
  strcpy (kit.name, "kit médico");
  strcpy (prego.description, "um kit médico usado para restaurar vida");
  kit.type[0] = 0;
  kit.type[1] = 5;
  kit.chance = 1;
  kit.damage = 0;
  kit.throwable[0], prego.throwable[1] = 0;
  kit.craftable[0], prego.craftable[1] = 0;
  kit.durability[0], prego.durability[1] = 0;
  kit.id = 10;

  Enemy wolf;
  strcpy (wolf.name, "LOBO");
  wolf.status[0] = 8;
  wolf.status[1] = 3;
  wolf.max_life = 8;
  wolf.chance = 4;
  wolf.in_hand = nada;
  wolf.body = 4;
  strcpy (wolf.parts[0], "patas");	/*trocar por pescoço, (descontinuado) */
  strcpy (wolf.parts[1], "pernas");
  strcpy (wolf.parts[2], "barriga");
  strcpy (wolf.parts[3], "cabeça");
  wolf.first_encounter[0] = 1;
  wolf.first_encounter[1] = 1;

  Enemy savage;
  strcpy (savage.name, "SELVAGEM");
  savage.status[0] = 12;
  savage.status[1] = 2;
  savage.max_life = 12;
  savage.chance = 3;
  savage.in_hand = pedra;
  savage.body = 4;
  strcpy (savage.parts[0], "pernas");
  strcpy (savage.parts[1], "corpo");
  strcpy (savage.parts[2], "braC'os");
  strcpy (savage.parts[3], "cabeC'a");
  savage.first_encounter[0] = 1;
  savage.first_encounter[1] = 2;


  Pool pool_1;
  pool_1.drops[0] = pedra;
  pool_1.drops[1] = prego;
  pool_1.drops[2] = kit;
  pool_1.drops[3] = batata;
  pool_1.encounter = wolf;

  Pool pool_2;
  pool_2.drops[0] = gasolina;
  pool_2.drops[1] = tabua;
  pool_2.drops[2] = pedra; // trocar por algo que aumente a força
  pool_2.drops[3] = lanca_ponta_de_madeira;
  pool_2.encounter = savage;

  Area area_1;
  strcpy (area_1.name, "arredores");
  area_1.risk = 1;
  area_1.loot = pool_1;

  Area area_2;
  strcpy (area_2.name, "matagal");
  area_2.risk = 2;
  area_2.loot = pool_2;

  Shack shack;
  strcpy (shack.name, "barraco");
  shack.bed_window[0], shack.bed_window[1] = 0;
  shack.living_window[0], shack.living_window[1] = 0;
  shack.kitchen_window[0], shack.kitchen_window[1] = 0;
  shack.living_door[0], shack.living_door[1], shack.living_door[2] = 0;
  shack.living_door[0], shack.living_door[1], shack.living_door[2] = 0;
  shack.back_door[0], shack.back_door[1], shack.back_door[2] = 0;
  shack.lamp_1[0], shack.lamp_1[1] = 0;
  shack.lamp_2[0], shack.lamp_2[1] = 0;
  shack.generator = 52;
  shack.hour = 6;
  int a;
  for (a = 0; a < 25; a++)
	{
	  shack.chest[a] = nada;
	}

  Player me;
  strcpy (me.name, "Artur");
  me.status[0] = 8; // vida
  me.status[2] = 8; // energia
  me.status[3] = 8; // Sanidade
  me.status[4] = 0; // notoriedade
  me.status[5] = 8; // fome
  me.status[1] = 3; // força
  me.max_life = 8;
  me.max_hunger = 8;
  me.max_energy = 8;
  me.in_hand = nada;
  int b;
  for (b = 0; b < 6; b++)
	{
	  me.bag[b] = nada;
	}

/*INICIO do game*/
  printf ("Aiii...\n");
  getchar ();
  printf ("Que dor de cabeça infernal!\n");
  getchar ();
  printf
	("Devo ter bebido até cair depois de matar uns nativos malditos.\n");
  getchar ();
  printf
	("Bebi tanto que me perdi no matagal, nem sei que choupana é essa.\n");
  getchar ();
  printf
	("Preciso achar um jeito de voltar pra casa antes que anoiteça...\n");
  getchar ();
  system("cls");
  score();
  getchar();
  system("cls");

  int c, d, e, f, g, h, i, j, k, l, m, n;


  do
	{
      game_over(&me);
	  if (shack.hour == 22) {
	      night(&me, &shack, wolf, savage, nada);
	  }

  	  system("cls");
      printf("+----------------------------------------------------------------------------+\n");
      printf("                                   %d:00\n", shack.hour);
	  printf("+----------------------------------------------------------------------------+\n");
      printf(",syQ$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$Ç$$$$$$$$$$$$$$$$Qys. \n");
      printf(":$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$P,d$$$$$$$$$$$$$$$$$$$: \n");
      printf("l$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$b,.`^²$$$$$$$$$$$$$$$$l \n");
      printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ⁿ"   "´_,,ssyyQQ$$$$$$$$$$$$$$$$$\n");
      printf("$$$$$$$$$$l$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ybs,,.._```^" "ᵘç$$$$$$$$$$$$$$$\n" );
      printf("$$$$$$$$$$:$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$P  _.,sy$$$$$$$$$$$$$$ \n");
      printf("$$$$$$$$s`:ᵘ,$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ç$$$$$$$" "`s$$$$$$$$$$$$$$$$$$$$$ \n");
      printf("$$$$$$$ys_  ,sl$$$$$$$$$$$$$$$$$$$$$$$$$$$$P´   ´´""ⁿç  d$$$$$$$$$$$$$$$$$$$$$ \n");
      printf("$$$$$$$çP` ` ,:$$$$$$$$$$$$$$$$$$$$$$$$$$P´ ,d.           ``^""²$$$$$$$$$$$$$$$ \n");
      printf("$$$$$$ᵘys, sdP ᵘ$$$$$$$$$$$$$$$$$$$$$$P´                        ²$$$$$$$$$$$$$ \n");
      printf("$$$$$$$$$$l$s, "",y$$$$$$$$$$$$$$$$$$$ys.                          ²$$$$$$$$$$$ \n");
      printf("$$l$$$l$$$:dy,  ,s$$$$l$$$$$$$$$$çᵘ²""                         ""` sy$$$$$$$$$$l \n");
      printf("$$:$$P:4$$ $$$ $$$$$$$:$$$$$$$$$$Qyys ss   ;$b                    `""ⁿᵘ$$$$$$$: \n");
      printf("sl `P  :$$ $$$:$$$$$$l l$²$$$$$$$$$$$ $$   $$$                     .s$$$$$$$l \n");
      printf("P   `   `ᵘ `ᵘcl$$$$$²   ` ´$$$$$$$$$$:$$                         ` :$$ç´4$$` \n");
      printf("+----------------------------------------------------------------------------+\n");
      printf("                                  ESCONDERIJO\n ");
      printf("+----------------------------------------------------------------------------+\n");
      printf("[1] status\n");
      printf("[2] baú \n");
      printf("[3] se preparar \n");
      printf("[4] bancada de trabalho \n");
      printf("[5] explorar \n");
      printf("[0] fechar \n");
      printf("\n");

	  printf ("\n-> ");
	  scanf ("%d", &c);
  	  system("cls");

	  if (c == 1) {
	      shack.hour ++;
	      do{
	      system("cls");
          printf("+----------------------------------------------------------------------------+\n");
          printf("Vida: %d \n", me.status[0]);
          printf("Fome: %d \n", me.status[5]);
          printf("Energia: %d \n", me.status[2]);
          printf("Sanidade: %d\n", me.status[3]);
          printf("Força: %d\n", me.status[1]);
          printf("Notoriedade: %d\n", me.status[4]);
          printf("----------------------- \n");
          printf("        MOCHILA \n");
          printf("----------------------- \n");
          printf("1 %s \n", me.bag[0].name);
          printf("2 %s \n", me.bag[1].name);
          printf("3 %s \n", me.bag[2].name);
          printf("4 %s \n", me.bag[3].name);
          printf("5 %s \n", me.bag[4].name);
          printf("MÃO(6) %s \n", me.in_hand.name);
          printf("+----------------------------------------------------------------------------+\n");
          printf("                                  ESCONDERIJO\n ");
          printf("+----------------------------------------------------------------------------+\n");
          printf("[1] usar item \n");
          printf("[2] descartar item \n");
          printf("[3] guardar na mochila \n");
          printf("[4] pegar da mochila \n");
          printf("[0] voltar \n");
          printf("\n");

		  printf ("\n-> ");
		  scanf ("%d", &d);
		  printf("\n");

		  if (d == 1) {
		      printf("Ok, qual vai ser?\n");
		      printf("\n->");
		      scanf("%d", &e);
		      if (e == 6) {
		          use_item_in_hand(&me, nada);
		      }

		      else {
		          use_item(e - 1, &me, nada);
		      }
		  }

		  if (d == 2) {
    		  printf("Ok, qual vai ser?\n");
    		  printf("\n->");
    		  scanf("%d", &f);
    		  if (f == 6) {
    		      drop_item_from_hand(&me, nada);
    		  }

    		  else {
    		      drop_item_from_bag(f - 1, &me, nada);
    		  }
		  }

		  if (d == 3) {
		      hand_to_bag(&me, nada);
		  }

		  if (d == 4) {
		      printf("Ok, qual vai ser?\n");
		      printf("\n->");
		      scanf("%d", &g);
		      bag_to_hand(g - 1, &me, nada);
		  }


		}while (d != 0);
	  }

	  if (c == 5) {
	      printf("\n[1] Arredores\n");
	      printf("[2] Matagal\n");
	      printf("\nOk, qual vai ser?\n");
		  printf("\n->");
		  scanf("%d", &k);

	      if (k == 1) {
	          explore(&me, area_1, nada);
	          shack.hour ++;
	      }

	      if (k == 2) {
	          explore(&me, area_2, nada);
	          shack.hour ++;
	      }

	  }

	  if (c == 4) {
	      shack.
	  	hour ++;
	  }

  	  if (c == 3) {
          shack.hour ++;
	  	  do {

            system("cls");

	  	  	printf("+----------------------------------------------------------------------------+\n");
            printf(",syQ$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$Ç$$$$$$$$$$$$$$$$Qys. \n");
  	  	    printf(":$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$P,d$$$$$$$$$$$$$$$$$$$: \n");
            printf("l$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$b,.`^²$$$$$$$$$$$$$$$$l \n");
            printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ⁿ"   "´_,,ssyyQQ$$$$$$$$$$$$$$$$$\n");
            printf("$$$$$$$$$$l$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ybs,,.._```^" "ᵘç$$$$$$$$$$$$$$$\n" );
            printf("$$$$$$$$$$:$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$P  _.,sy$$$$$$$$$$$$$$ \n");
            printf("$$$$$$$$s`:ᵘ,$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$ç$$$$$$$" "`s$$$$$$$$$$$$$$$$$$$$$ \n");
            printf("$$$$$$$ys_  ,sl$$$$$$$$$$$$$$$$$$$$$$$$$$$$P´   ´´""ⁿç  d$$$$$$$$$$$$$$$$$$$$$ \n");
            printf("$$$$$$$çP` ` ,:$$$$$$$$$$$$$$$$$$$$$$$$$$P´ ,d.           ``^""²$$$$$$$$$$$$$$$ \n");
            printf("$$$$$$ᵘys, sdP ᵘ$$$$$$$$$$$$$$$$$$$$$$P´                        ²$$$$$$$$$$$$$ \n");
            printf("$$$$$$$$$$l$s, "",y$$$$$$$$$$$$$$$$$$$ys.                          ²$$$$$$$$$$$ \n");
            printf("$$l$$$l$$$:dy,  ,s$$$$l$$$$$$$$$$çᵘ²""                         ""` sy$$$$$$$$$$l \n");
            printf("$$:$$P:4$$ $$$ $$$$$$$:$$$$$$$$$$Qyys ss   ;$b                    `""ⁿᵘ$$$$$$$: \n");
  	  	    printf("sl `P  :$$ $$$:$$$$$$l l$²$$$$$$$$$$$ $$   $$$                     .s$$$$$$$l \n");
            printf("P   `   `ᵘ `ᵘcl$$$$$²   ` ´$$$$$$$$$$:$$                         ` :$$ç´4$$` \n");
            printf("+----------------------------------------------------------------------------+\n");
            printf("                           REFORCE O ESCONDERIJO\n ");
            printf("+----------------------------------------------------------------------------+\n");
            printf("[1] porta da sala -> por barricada S/N? \n");
            printf("[2] janela da sala -> por barricada S/N? \n");
            printf("[3] janela da cozinha -> por barricada S/N? \n");
            printf("[4] gerador -> %d \n", shack.generator);
	  	  	printf("[0] sair \n");

  	  	    printf ("\n-> ");
  	  	    scanf ("%d", &l);
  	  	    system("cls");

  	  	    if (l == 1 && search_item(tabua, me) != -1 && shack.living_door[0] == 0) {
	  	  	    shack.living_door[0] = 1;
  	  	   	    int bagIndex = search_item(tabua, me);
  	  	   	    drop_item_from_bag(bagIndex, &me, nada);
  	  	   	    print("A porta está barricada.");
  	  	   }

	  	  	if (l == 2 && search_item(tabua, me) != -1 && shack.living_window[0] == 0) {
	  	  		shack.living_window[0] = 1;
	  	  		int bagIndex = search_item(tabua, me);
	  	  		drop_item_from_bag(bagIndex, &me, nada);
	  	  		print("A janela da sala está barricada.");
	  	  	}

	  	  	if (l == 3 && search_item(tabua, me) != -1 && shack.kitchen_window[0] == 0) {
	  	  		shack.kitchen_window[0] = 1;
	  	  		int bagIndex = search_item(tabua, me);
	  	  		drop_item_from_bag(bagIndex, &me, nada);
	  	  		print("A janela da cozinha está barricada.");
	  	  	}

	  	  	if (l == 4 && search_item(gasolina, me) != -1) {
	  	  		shack.generator += 25;
	  	  		int bagIndex = search_item(gasolina, me);
	  	  		drop_item_from_bag(bagIndex, &me, nada);
	  	  		print("Abasteci um pouco do gerador.");
	  	  	}




	  	  }while(l != 0);

  	  }


	  if (c == 2) {
	      shack.hour ++;
	      do {
	          system("cls");
	          printf("----------------------- \n");
              printf("        MOCHILA \n");
              printf("----------------------- \n");
              printf("1 %s \n", me.bag[0].name);
              printf("2 %s \n", me.bag[1].name);
              printf("3 %s \n", me.bag[2].name);
              printf("4 %s \n", me.bag[3].name);
              printf("5 %s \n", me.bag[4].name);
              printf("MÃO(6) %s \n", me.in_hand.name);
              printf("+----------------------------------------------------------------------------+\n");
              printf("                                     BAÚ\n ");
              printf("+----------------------------------------------------------------------------+\n");
              printf("\n");
              printf("1 %s               6 %s\n", shack.chest[0].name, shack.chest[5].name);
              printf("2 %s               7 %s\n", shack.chest[1].name, shack.chest[6].name);
              printf("3 %s               8 %s\n", shack.chest[2].name, shack.chest[7].name);
              printf("4 %s               9 %s\n", shack.chest[3].name, shack.chest[8].name);
              printf("5 %s               10 %s\n", shack.chest[4].name, shack.chest[9].name);
              printf("\n");
              printf("11 %s              16 %s\n", shack.chest[10].name, shack.chest[15].name);
              printf("12 %s              17 %s\n", shack.chest[11].name, shack.chest[16].name);
              printf("13 %s              18 %s\n", shack.chest[12].name, shack.chest[17].name);
              printf("14 %s              19 %s\n", shack.chest[13].name, shack.chest[18].name);
              printf("15 %s              20 %s\n", shack.chest[14].name, shack.chest[19].name);
              printf("\n");
              printf("21 %s\n", shack.chest[20].name);
              printf("22 %s\n", shack.chest[21].name);
              printf("23 %s\n", shack.chest[22].name);
              printf("24 %s\n", shack.chest[23].name);
              printf("25 %s\n", shack.chest[24].name);
              printf("+----------------------------------------------------------------------------+\n");
              printf("[1] por no baú\n");
              printf("[2] pegar do baú\n");
              printf("[0] voltar\n");
              printf("\n");


              printf ("\n-> ");
		      scanf ("%d", &h);
		      printf("\n");

		      if (h == 1) {
		          printf("Ok, qual vai ser?\n");
		          printf("\n->");
		          scanf("%d", &i);

		          if (i == 6) {
		              hand_to_chest(&shack, &me, nada);
		          }

		          else {
		              bag_to_chest(i - 1, &shack, &me, nada); // ele bota 1 item e para de funcionar?? (resolvido)
		          }
		      }

		      if (h == 2) {
		          printf("Ok, qual vai ser?\n");
		          printf("\n->");
		          scanf("%d", &j);
		          chest_to_bag(j - 1, &shack, &me, nada);
		      }

	      }while (h != 0);
	  }

	} while (c != 0);

  return 0;
}