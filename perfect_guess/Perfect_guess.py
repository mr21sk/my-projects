import random 

f = open("rules.txt",'r')
data = f.read()
print(data)
f.close()

import random

comp_int = random.randint(1, 100)
print("Guess the number between 1 and 100. You have 3 attempts!")

for i in range(3):  
    user_choice = int(input(f"Attempt {i + 1}: Enter your guess: "))
    
    if user_choice == comp_int:
        print("You Win, Boss! 🎉")
        break

    elif user_choice > comp_int:
        print("Your guess is too high.")
        
    elif user_choice < comp_int:
        print("Your guess is too low.")
    
    if i == 2:  
        print(f"Sorry, Boss! You lose this time. The correct number was {comp_int}. Better luck next time!")


