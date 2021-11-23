#include <iostream>
#include <string>
#include <ctime>
using namespace std;
const int NUMBER_POKEMON = 6;
const int NUMBER_OF_MOVES = 4;
enum class Type { Fire, Water, Grass, Normal }; // Use enum for types. This makes the code easier, and we don't need string comparissons.
class Move //Move class: responsible for a Move that a pokemon can use in battle
{
private:
    string name; //the name of the move
    int attackValue; // the moves attack value
    int totalPowerPoints; //the total ammount of times the move can be used;
    int currentPowerPoints; //the ammounf of moves left
    Type type;
public: 
    Move()
    {
        name = "";
        attackValue = currentPowerPoints = totalPowerPoints = 0;
    };
    Move(string n, int av, int pp, Type t)
    {
        name = n;
        attackValue = av;
        totalPowerPoints = currentPowerPoints = pp;
        type = t;
    }
    int useMove()//using a move reeduces the current power points by 1 and generates an attack value;
    {
        if (currentPowerPoints <= 0)
        {
            cout << "No PP left!" << endl;
            return 0;
        }
        else
        {
            currentPowerPoints -= 1;
            int damage = (5 + rand() % attackValue); //the attack value is a random number between 5 and the top value for attack
            return damage;
        }
    }

    string getName() //return the name of the move
    {
        return name;
    }
    Type getType() //return the type of the move
    {
        return type;
    }
    string getPPremainng() //return a string the says how much PP is remaining from the total PP
    {
        return to_string(currentPowerPoints) + "/" + to_string(totalPowerPoints) + " PP remaing";
    }

};
class Pokemon
{
private:
    string name; //the pokemons name
    int level; //the pokemons level, this is used to caclulate damage for an attack
    int health;// the pokemons current health, if this gets to 0 they are knocked out
    int maxHealth; // the maximum health the pokemon can have
    Type type;// the pokemons elemental type, this is used in attacking to check for effectiveness
    bool knockedOut; // if this is true, the pokemon is kockedout (note this could be replace with just a fuction that checks if helth is less than 0 but I wanted to have a varaible)
    Move moves[NUMBER_OF_MOVES]; //a pokemon can have four moves
public:
    Pokemon()
    {
        name = "";
        level = 1;
        health = 1;
        maxHealth = 1;
        type = Type::Normal;
        knockedOut = false;
    }
    //To create a pokemon, give it a name, type, and level. Its starting health is its max health and it is not knocked out when it starts.
    Pokemon(string n, int l, int mh, Type t, Move m[NUMBER_OF_MOVES])
    {
        name = n;
        level = l;
        health = mh;
        maxHealth = mh;
        type = t;
        knockedOut = false;
        for (int i = 0; i < NUMBER_OF_MOVES; i++) //copy the array being passed in to the array in the class
        {
            moves[i] = m[i];
        }

        srand(time(NULL));
    }
    string getType() //return the pokemon type as a string for display to user
    {
        if (type == Type::Fire)
        {
            return "Fire";
        }
        else if (type == Type::Water)
        {
            return "Water";
        }
        else
        {
            return "Grass";
        }
    }
    //Printing a pokemon will give you all the info about the pokemon: its name, its type, its level and how much health it has remaining
    void display()
    {
        cout << "This level " << level << " " << name << " has " << health << " hp remaining , they are a " << getType() << " type pokemon" << endl << endl;
    }

    void revive()//Reviving a pokemon will set knockedOut to false
    {
        knockedOut = false;
        if (health <= 0) { //safety check, if the pokemon has somehow been revived but the health is still 0 or less then set it to 1.
            health = 1;
        }
        cout << name << " was revived!" << endl;
    }

    void knockOut()//Knocking out a pokemon change knockedOut to true
    {
        knockedOut = true;
        //A knocked out pokemon can't have any health.
        health = 0;
        cout << name << " was knocked out!" << endl;
    }

    void loseHealth(int amount) //Deducts heath from a pokemon and prints the current health reamining
    {

        health -= amount;
        if (health <= 0)
        {// Makes sure the health doesn't become negative. Knocks out the pokemon.
            health = 0;
            knockOut();
        }
        else
        {
            cout << name << " now has " << health << " remaining " << endl;
        }
    }

    void gainHealth(int amount)//Adds to a pokemon's health
    {
        //If a pokemon goes from 0 heath, then revive it
        if (health == 0)
        {
            revive();
        }
        health += amount;
        //Makes sure the heath does not go over the max health
        if (health >= maxHealth)
        {
            health = maxHealth;
        }
        cout << name << " now has " << health << " remaining " << endl << endl;
    }

    //attack the other pokemon in the battle
    void attack(Pokemon& otherPokemon) //important that this is passed by reference
    {
        if (knockedOut)//Checks to make sure the pokemon isn't knocked out
        {
            cout << name << " can't attack becuase it is knocked out " << endl;
            return;
        }
        cout << name << "'s Moves:" << endl; //list each move for the pokemon
        for (int i = 0; i < NUMBER_OF_MOVES; i++)
        {
            cout << i + 1 << ": " << moves[i].getName() << " " << moves[i].getPPremainng() << endl;
        }

        int moveChoice = 0;
        while (moveChoice < 1 || moveChoice >4) //get the user to select a valid move
        {
            cout << "Which move will you use?" << endl;
            cin >> moveChoice;
        }
        int damage = moves[moveChoice - 1].useMove(); //use the move and return the damage value
        cout << name << " attacked " << otherPokemon.name << " with " << moves[moveChoice - 1].getName() << " it did " << damage << " damage " << endl;
        if ((moves[moveChoice - 1].getType() == Type::Fire && otherPokemon.type == Type::Water) || (moves[moveChoice - 1].getType() == Type::Water && otherPokemon.type == Type::Grass) || (moves[moveChoice - 1].getType() == Type::Grass && otherPokemon.type == Type::Fire))
        {//damage dealt is a random number between 1 and the level
            damage /= 2; //for weak type it is half;            
            cout << "Its not very effective." << endl;
            otherPokemon.loseHealth(damage);
        }//f the pokemon attacking has neither advantage or disadvantage then there is no modifier
        else if (moves[moveChoice - 1].getType() == otherPokemon.type || moves[moveChoice - 1].getType() == Type::Normal) //in this example, normal has no effect on damage increase/ decrase
        {//damage dealt is a random number between 1 and the level
            otherPokemon.loseHealth(damage);
        }//If the pokemon attacking has advantage, then it deals double damage
        else if ((moves[moveChoice - 1].getType() == Type::Fire && otherPokemon.type == Type::Grass) || (moves[moveChoice - 1].getType() == Type::Water && otherPokemon.type == Type::Fire) || (moves[moveChoice - 1].getType() == Type::Grass && otherPokemon.type == Type::Water))
        {//damage dealt is a random number between 1 and the level
            damage *= 2; //times 2 for strong type           
            cout << "Its super effective." << endl;
            otherPokemon.loseHealth(damage);
        }
        cout << endl;
    }
    string getName() //return the pokemons name
    {
        return name;
    }
    bool isKnockedOut() //check if the pokemon is knocked out
    {
        return knockedOut;
    }
};
class Trainer
{
private:
    //A trainer has a one pokemon, a number of potions and a name. When the trainer gets created, they are assigned a pokemon
    Pokemon pokemon[NUMBER_POKEMON]; // a trainer can have siz pokemon
    string name; //the trainers name, purely for display purposes
    int numOfPotions; //the number of potions and trainer hyas
    int currentPokemon; //this is the value that indicates the current pokemon the trainer is using from the array.

public:
    Trainer(Pokemon p[NUMBER_POKEMON], string n, int pot)
    {
        for (int i = 0; i < NUMBER_POKEMON; i++)
        {
            pokemon[i] = p[i];
        }
        name = n;
        numOfPotions = pot;
        currentPokemon = 0;
    }
    void display()
    {//display the trainer name and thier pokemon to the screen
        cout << "Trainer " << name << " has the pokemon " << pokemon[currentPokemon].getName() << endl;
    }

    void usePotion()
    {//Uses a potion on pokemon, assuming you have at least one potion.

        if (numOfPotions > 0)
        {
            cout << name << " Used a potion ";
            //decrement number of potions
            numOfPotions -= 1;
            //a potion restores 20 health;
            pokemon[currentPokemon].gainHealth(20);
        }
        else
        {
            cout << "You do not have any potions" << endl;
        }

    }
    void attackOtherTrainersPokemon(Trainer& otherTrainer) //trainer is passed by reference
    {//attack the other trainers pokemon;
        pokemon[currentPokemon].attack(otherTrainer.getCurrentPokemon());
    }
    Pokemon& getCurrentPokemon() //return the pokemon object as a reference
    {
        return pokemon[currentPokemon]; //get the current pokemon from the array of pokemon
    }
    string getName() //return the trainers name
    {
        return name;
    }
    int getNumOfPotions()// return the number of potions, this is used as a check alongside if the pokemon is knocked out
    {
        return numOfPotions;
    }
    void switchPokemon() //updates the currentPokemon value so that the trainer can switch pokemon
    {
        bool invalidChoice = true; //create a loop to make sure the user picks a correct input
        while (invalidChoice)
        {
            cout << "Which pokemon would you like to switch to: " << endl; //ask the user for the input
            for (int i = 0; i < NUMBER_POKEMON; i++) //list all the pokemon they have
            {
                if (pokemon[i].getName() != "") //only if the name is not blank (a trainer can have up to 6 pokemon)
                {
                    cout << i + 1 << ": " << pokemon[i].getName() << endl;
                }

            }
            int userInput;
            cin >> userInput; //get the users input

            currentPokemon = userInput - 1; //change the current Pokemon value to the userinput -1 (because of arrays)
            if (pokemon[currentPokemon].getName() == "") //if the pokemons name is empty then it is not a valid choice
            {
                cout << "Invalid choice, please choose again" << endl; //tell the user to try again
            }
            else
            {
                cout << "You have switch to : " << pokemon[currentPokemon].getName() << endl; //if it is a valid choice, then inform them they have switched, and stop the loop
                invalidChoice = false;
            }

        }

    }

    bool areAllPokemonKnockedOut() //check if all pokemon are knocked out
    {
        bool allKnockedOut = true; //set the return variable to true
        for (int i = 0; i < NUMBER_POKEMON; i++)//loop over all pokemon
        {
            if (pokemon[i].getName() != "")//only check real pokemon
            {
                if (!pokemon[i].isKnockedOut())//if any of the pokemon are not knocked out then set the knocked out variable to false.
                {
                    allKnockedOut = false;
                }
            }
        }
        return allKnockedOut;
    }
};
//trainers are passed by reference, so that the values are operated on correctly.
void getBattleAction(Trainer& trainer1, Trainer& trainer2)
{//get the trainers input for attack or use potion
    int trainerInput;
    cout << trainer1.getName() << ": Choose your action:" << endl;
    cout << "1. Attack " << endl;
    cout << "2. Potion: You have " << trainer1.getNumOfPotions() << " potions remaining" << endl;
    cout << "3. Switch Pokemon: " << endl;
    cout << "3. Run!: " << endl;
    cin >> trainerInput;
    switch (trainerInput)
    { //if the trainer uses 1,the attack the other trainers pokemon
    case 1:
        trainer1.attackOtherTrainersPokemon(trainer2);
        break;
    case 2://if the trainer uses 2, the use a potion ont thier own potion
        trainer1.usePotion();
        break;
    case 3://if the trainer uses 2, the use a potion ont thier own potion
        trainer1.switchPokemon();
        break;
    case 4:
        cout << "You couldn't get away" << endl;
    default:
        break;
    }
}

int main()
{//Three objects that are of Pokemon type. Charmander is a fire type, Squirtle is a Water type, and Bulbasaur is a Grass type.
    //create some move objects
    Move tackle("Tackle", 10, 30, Type::Normal);
    Move scratch("Scratch", 12, 30, Type::Normal);

    Move ember("Ember", 20, 20, Type::Fire);
    Move flamethrower("Flamethrower", 30, 15, Type::Fire);

    Move waterGun("Water Gun", 20, 20, Type::Water);
    Move bubbleBeam("Bubblebeam", 30, 15, Type::Water);

    Move vineWhip("Vine Whip", 20, 20, Type::Grass);
    Move razorLeaf("Razor Leaf", 30, 15, Type::Grass);

    Move hydroPump("Hydro Pump", 50, 5, Type::Water);
    Move hyperBeam("Hyper Beam", 60, 5, Type::Normal);
    Move bite("Bite", 25, 15, Type::Normal);
    //create movesets
    Move charmandersMoves[NUMBER_OF_MOVES] = { tackle,scratch,ember,flamethrower };
    Move squirtlesMoves[NUMBER_OF_MOVES] = { tackle,scratch,waterGun,bubbleBeam };
    Move bulbasaursMoves[NUMBER_OF_MOVES] = { tackle,scratch,vineWhip,razorLeaf };
    Move gyradosMoves[NUMBER_OF_MOVES] = { tackle, bite, hydroPump,hyperBeam };
    //create pokemon
    Pokemon charmander("Charmander", 10, 50, Type::Fire, charmandersMoves); //constructur is the name, the level, the health and the type, and the move set
    Pokemon Squirtle("Squirtle", 12, 60, Type::Water, squirtlesMoves);
    Pokemon Bulbasaur("Bulbasaur", 8, 40, Type::Grass, bulbasaursMoves);
    Pokemon Gyrados("Gyrados", 20, 100, Type::Water, gyradosMoves);

    //invite trainers to enter thier names
    cout << "Welcome to pokemon battle simulator" << endl;
    cout << "Trainer 1: Please enter your name " << endl;
    string trainer1name;
    getline(cin, trainer1name);
    Pokemon trainer1sPoke[NUMBER_POKEMON] = { charmander, Gyrados };

    Trainer trainer1(trainer1sPoke, trainer1name, 5);

    cout << "Trainer 2: Please enter your name " << endl;
    string trainer2name;
    getline(cin, trainer2name);

    cout << endl;

    Pokemon trainer2sPoke[NUMBER_POKEMON] = { Bulbasaur,Squirtle };
    Trainer trainer2(trainer2sPoke, trainer2name, 5);
    //display trainer information to the screen
    trainer1.display();
    trainer1.getCurrentPokemon().display();

    trainer2.display();
    trainer2.getCurrentPokemon().display();
    bool stillBattling = true;


    while (stillBattling) //keep the game loop going
    {
        getBattleAction(trainer1, trainer2);
        getBattleAction(trainer2, trainer1);// if a pokemon is knocked out then declare the other trainer the winner

        if (trainer1.areAllPokemonKnockedOut() && trainer1.getNumOfPotions() == 0) //if a pokemon is knocked out then end the battle and the trainer as no potions then it is game over
        {
            stillBattling = false; //the battle will only end if a pokemon is knocked out and the trainer as no potions
            cout << trainer1.getName() << "'s pokemon is knocked out " << trainer2.getName() << " wins" << endl;
        }
        else if (trainer2.areAllPokemonKnockedOut() && trainer2.getNumOfPotions() == 0)
        {
            stillBattling = false; //the battle will only end if a pokemon is knocked out and the trainer as no potions
            cout << trainer2.getName() << "'s pokemon is knocked out " << trainer1.getName() << " wins" << endl;
        }
    }
}


