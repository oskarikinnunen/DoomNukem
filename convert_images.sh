if [ "$1" == "re" ]; then
	echo "Reconverting images:"
	cd assets && bash reconvert.sh && cd .. && cd ..
else
	echo "Converting images:"
	cd assets && bash convert.sh && cd .. && cd ..
fi


