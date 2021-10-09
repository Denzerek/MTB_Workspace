#!/bin/bash


read -p 'Commit Message : ' test

#Add authentication agent
eval "$(ssh-agent -s)"

#add the path from the above .pub file to ssh 
ssh-add /c/Users/Austina/.ssh/Denzerek_key


git add . && git commit -m "$test" && git push -u origin main
