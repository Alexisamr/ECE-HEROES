main{
    int choix;
	do{
		choix = affichermenu();
        switch (choix){
	      case 1:
	        afficherRegles();
	        break;
	      case 2:
	        //fonctione nouvelle partie();
	        break;
	      case 3:
	        //fonction reprendre partie();
	        break;
	      default;
	        break;
    	}
	}while(choix != 0);
	return 0;
}

gererMenu{

}

nouvellePartie{

} 

reprendrePartie{

}

lancerNiveau{

}

finPartie{
    
}
