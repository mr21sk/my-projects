import random

#Instructions
print(''' Instructions For the game \" Stone Paper Scissor \" :
     Let -1 = Scissor ,
          0 = Stone ,
          1 = Paper .
     Enter your choice from either -1 or 0 or 1 .
     if your is same as computer then it is a draw .
     if your choice is Scissor and computer chooses : 1) Stone then, you lose.
                                                      2) Paper then, you win. 
     if your choice is Stone and computer chooses : 1) Scissor then, you win.
                                                    2) Paper then, you lose.
     if your choice is Paper and computer chooses : 1) Stone then, you win.
                                                    2) Scissor then, you lose.''')
#choice generation
computer = random.randint(-1,1)
user = int(input("Enter your choice : "))
print(f"Your choice is {user}")
print(f"Computer's choice is {computer}")

#Result 
if (computer == user) :
    print("The match is draw.")
elif (computer==1 and user ==-1) or (computer==-1 and user ==0) or (computer==0 and user ==1):
    print("you win")
elif (computer==0 and user ==-1) or (computer==1 and user ==0) or (computer==-1 and user ==1):
    print("you lose")
else:
    print("Invalid Choice")

