commamd to compile
g++ List_Multilane.h List_Carro.cpp List_Carril.cpp List_Avenida.cpp Dinamica.cpp List_Implementation.cpp -o 1.x

command to erase all data files with a given name
find . -name "name" -delete

Observations:
1) If the car is allowed to accelerate when gap=gap_acceleration then the flux vs density diagram has a different behaviour
	than if it is not allowed to do it!!!!!!
2) Info about the max desceleration of the cars https://copradar.com/chapts/references/acceleration.html

Done:
1) Create a function to stop a given car or a car that passes through a given point.
2) Add a condition in the Decide_Change_Lines_And_Direccionales function such that if the car ahead has the
	parking lights on then the car ask for the way with the turning lights, the way is asked when the car ahead
	is close enough
3) Create a function to get the information about the cars that leave the lane.
4) Create a function to export de dats from the cars that leave the lane
5) Randomize the Change_Lines protocol such that if the incentive is the same in both lanes then with
	a random number generator the lane is chossed randomly.
6)


Agregar la cantidad de carros que se mueven del carril
Pending:
2) Review the past results with the new implementation:
	a) Flux vs Density diagram for the single lane model ---> inconsistencies with Olmos-Muñoz model hence the implemented model 
		is used.
	b) Flux vs Density diagram for the lane with each of the change lanes incentives.
	c) Flus vs Density diagram for each change lines incentive with the kindness parameter different from zero.
	d) Flux vs Density diagram for one car stopped and two lines and how it varies when using periodic boundaries
		so the dependence with the system size can be stabished and the open boundaries are justified.
	e) Flux vs Density diagram with the open boundaries using no multilane and multilane models so the 
		open boundaries can be demostrated to generate the dessire density inside the simulaiton zone.
	f) Check open boundaries with one stopped lane and generate the flux vs density profile for both open and 
		periodic boundaries.
	g) Check the variation of the flux vs density profile for different values of the knidness to give the way.

3) Divide histograms between the data of the cars that are fast and the cars that are slow
4) Calculate the mean and standart deviation of the fast cars ans slow cars as a function of the knidness
5) Apply normality tests to the data of the fast cars and the slow cars; shapiro wilky test
6) Proportion of fast cars and slow cars


Problems:
1) The flux vs density diagram is not the same for the boundary lanes and the simulation lanes without neighbouring 
	interactions.

To Review:
1) The open boundaries conditions are stablished, check if they are consistent since they produce a  simulation where 
cars do not collide.

Observations:
1) When creating the names of the files if those are to be analyzed with xmgrace then use short names.

Proposals:
1) For the initial positions a list with all the free positions can be created so that the position are organized 
in ascending order and then a random number less than the amount of elements in the list be generated such that
the position in that element is filled with a car. When creating a car the positions that cannot be occupied are
eliminated from the list.




