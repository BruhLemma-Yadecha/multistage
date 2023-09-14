# multistage
A command-line multi-stage rocket optimizer built in C. Superseded by a [newer version](https://github.com/BruhLemma-Yadecha/rocketmancer) that uses simulated annealing, and comes with immensely improved performance and integration of object-oriented design.

# Usage
Base configuration of the rocket intended can be input via a CSV file, as per the sample document included. Generally, the increments required and base details are provided, while stage details are provided sequentially for qualities such as specific impulse and mass fraction. The program then provides a report detailing the most optimal configuration found, in addition to a file lisitng all of the optimisiation steps taken (useful for locating adjacent builds that may be more realistic).

# Planned Features
- Completed integration to a web interface via Flask.
