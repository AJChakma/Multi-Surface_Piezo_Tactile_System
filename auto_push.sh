#!/bin/bash
#Script qui add et push automatiquement sur le repo.
#Script un peu moyen je l'admet mais ça peut servir ?
user_repository="https://github.com/AJChakma/Multi-Surface_Piezo_Tactile_System"
user_files="README.md src/ incld/ TapZone.pro auto_push.sh"
user_message=""
user_branch="master"
git add $user_files
echo "Repository : $user_repository"
echo "Commit message :  (default : nothing) "
read user_message

if [ -z "$user_message" ] ; then
	git commit 
else
	git commit -m "$user_message"
fi

echo "Branch : (default : master) "
read user_branch
if [ -z "$user_branch" ] ; then
	user_branch="master"
fi

git push $user_repository $user_branch



