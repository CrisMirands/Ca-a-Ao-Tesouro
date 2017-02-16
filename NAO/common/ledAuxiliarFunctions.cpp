//#include <includeMapBehavior.hpp>
#include <ledAuxiliarFunctions.hpp>

namespace LEDAux
{
    void changeColor (int Chest, int REye , int LEye)
	{
         AL::ALLedsProxy led("127.0.0.1");

	    switch(Chest)
	    {
	    	case 0:
                led.off("ChestLeds"); //off
		    	break;
            case 1:
                led.on("ChestLedsRed"); //red
		    	break;
		    case 2:
                led.on("ChestLedsGreen"); //green
		    	break;
		    case 3:
                led.on("ChestLedsBlue");  //blue
		    	break;
	    }

	    switch(REye)
	    {
	    	case 0:
                led.off("RightFaceLeds"); //off
		    	break;
		    case 1:
                led.on("RightFaceLedsRed"); //red
		    	break;
		    case 2:
                led.on("RightFaceLedsGreen");  //green
		    	break;
		    case 3:
                led.on("RightFaceLedsBlue"); //blue
		    	break;
	    }

	    switch(LEye)
	    {
        case 0:
            led.off("LeftFaceLeds"); //off
            break;
        case 1:
            led.on("LeftFaceLedsRed"); //red
            break;
        case 2:
            led.on("LeftFaceLedsGreen");  //green
            break;
        case 3:
            led.on("LeftFaceLedsBlue"); //blue
            break;
	    }
	}
}
