# Ski Lift Simulator — Original Assignment

## Overview

The task is to build a simulation of a ski resort with ski lifts, slopes, queues, tickets, arrivals, departures, and a pseudo-graphical console output.

---
## Ski Resort Scenario

In the village of **Schneedorf**, there is a small ski mountain with:

- **2 blue slopes**
- **2 red slopes**
- **1 black slope**

A ski lift transports skiers through the resort:

- At the **middle station**, skiers can access:
    - **B1**
    - **R1**
- If they stay on the lift, they continue to the **summit station**, where they can access:
    - **B2**
    - **R2**
    - **S1**

At the **summit station**, everyone must get off.

---
## Lift Behaviour

At the summit station, timid skiers (“scaredy-cats”) may ride the lift back down.

At the middle station:

- scared skiers coming down from the summit may get off
- new scared skiers may board and continue down to the valley station

At the **valley station**, everyone must get off.

### Lift travel times

- Valley → Middle: **4 minutes**
- Middle → Summit: **5 minutes**

### Gondola spacing and total number of gondolas

The lift uses **4-person gondola** that are attached at equal distances along the cable.

- One chair arrives every **10 seconds**
- Therefore:
    - **54 chairs** are always traveling upward
    - **54 chairs** are always traveling downward
- In the turnaround loops, there are **2 additional chairs**

**Total gondolas in the system: 110**

### Boarding and exiting

- Boarding and exiting happen **while the lift is moving**
- The lift **never stops**
- Assume there are **no accidents**
- The emergency stop is never needed

### Gondola capacity

Each chair may carry **at most 4 people**.

Example:  
If a chair arrives at the valley station with 3 people already on it, and nobody gets off at the middle station, then only **1 more person** may board for the remaining ride.

---
## Slopes

The slope travel times are given as:

- **fastest time**
- **average time**

There is **no maximum time**.

### Slopes and travel times

- **B1** (middle station): **3 / 11 minutes**
- **B2** (summit station): **5 / 21 minutes**
    - then continues via **B1 or R1**
- **R1** (middle station): **3 / 10 minutes**
- **R2** (summit station): **4 / 19 minutes**
    - then continues via **B1 or R1**
- **S1** (summit station): **9 / 34 minutes**
    - does **not** pass through the middle station

A skier may take much longer than average.  
If a skier needs 3 hours for a slope, that is valid.

---
## Skiing Behaviour

Each slope run must choose:

- a **random slope**
- a **random descent duration**

The minimum time for the slope may **never be undercut**.

### Skill model

Skiers may have a skill level and should generally avoid slopes above their ability.

Example:

- a blue-level skier might try a red slope
- but should avoid the black slope

---
## Tickets

Skiers can buy two kinds of tickets.

### Ten-ride ticket

Valid for **10 lift rides**.  
Each boarding costs **1 ride**.

Possible rides that each count as 1:

- Valley → Middle
- Valley → Summit
- Middle → Summit
- Middle → Valley
- Summit → Middle
- Summit → Valley

### Day ticket

Valid until the ski lift closes.

### Ticket rules

- Tickets are **bound to a person**
- Tickets are **not transferable**
- No time is lost buying tickets
- Online tickets and highly trained staff make the process instantaneous

---
## Opening and Closing Times

### Lift opening hours

- Opens: **09:00**
- Runs continuously until: **20:00**

### Mountain closing time

- Entire mountain closes at: **22:00**

Any skiers still on slopes at 22:00 are brought down to the valley station by ski patrol using a **snowmobile**.

---
## How Skiers Arrive

Skiers arrive at the resort in several ways.

### On foot

Single skiers may walk from Schneedorf.

- Schneedorf has at most **60 hotel beds** for vacation guests

### By car

Groups of **1 to 5 skiers**

- Parking/stopping is forbidden everywhere except at the lift
- There is one parking area with capacity for **50 cars**

### By bus

Up to **50 skiers** per bus

- First bus arrives at **08:50**
- First bus departs at **08:53**
- Then a bus arrives every **20 minutes**
- Each bus departs again **3 minutes later**
- Last bus arrives at **20:10**
- Last bus departs at **20:13**

---
## Departure Rules

Skiers must leave the mountain using the **same transport mode** by which they arrived.

Examples:

- If 314 skiers arrived by car, then 314 skiers must also leave by car
- They must leave in the **same car** they arrived in
- Bus passengers must leave by bus
- Walk-in guests leave on foot
- Hotel guests return to the hotel

Assume all bus passengers plan their day intelligently enough to catch a return bus.

---
## Visitor Density Over the Day

Traffic is low in the morning and evening.  
Peak time is from **11:00 to 17:00**.

For simplicity, assume:

- **linear increase** up to peak time
- **linear decrease** after peak time

---
## Bistro

There is a small bistro at the middle station where skiers can buy snacks.

- No seating is available
- Skiers must stand outside in the cold to eat/drink
- Nobody stays there voluntarily for more than **15 minutes**
- Ordering and receiving food takes **2 minutes**

After eating, guests should **not immediately line up at the bistro again**.

---
## Information That Must Always Be Visible

The simulator must provide a **pseudo-graphical console output**.

This means:

- text and graphic-like elements appear in fixed positions
- the simulation output should look like a stable console dashboard

At program start, the simulation should run for **one full day**.


The simulation output must always show:
### Queue lengths

All queue lengths must be visible at all times, including:

- valley station lift queue
- middle station upward queue
- middle station downward queue
- summit station queue
- bus stop queue

### Number of skiers on each slope

For all five slopes:

- B1
- B2
- R1
- R2
- S1

### Ticket sales totals

- total number of **10-ride tickets sold**
- total number of **day tickets sold**

### Parking occupancy

- current number of parked cars

### Bus status

It must be visible when a bus:

- arrives
- waits
- departs

### Total number of people currently on the mountain

This includes:

- people in the lift
- people at the bistro
- people on slopes
- people in all queues

### Total number of ski runs

The total number of ski runs for the day must be counted cumulatively.

Important:
- timid skiers riding the lift back down do **not** count as ski runs
- a ten-ride ticket does **not** necessarily equal 10 ski runs
- a skier could ride only partway, get off, and board again

---
## Simulation Speed and Controls

The simulator must **not run in real time**.

### Standard speed

- one real-time second equals one simulated minute

### Turbo mode

- On key press, the simulation must speed up by a factor of **10**

### Pause

- The simulation must be pausable at any time by key press

### Abort

- The simulation must be terminable at any time by key press

---
## Example Output Requirements

The exact visual style may vary, but the following information must be represented clearly:

- sold 10-ride tickets
- sold day tickets
- total ski runs
- lift occupancy between valley/middle and middle/summit
- skier counts on all slopes
- queue lengths at all stations
- bistro occupancy
- current time
- number of people on the mountain
- parking occupancy
- bus presence/status
- controls such as turbo, pause, and abort