# Description des messages


## Message moteur

Les elecs vous laissent gentimment choisir la convention qu'il vous plaît pour le sens de rotation.

### Moteur asservis

ID : 0x05

* Position en angle : 
  * CMD : 0x01
  * Data : id du moteur sur 8 bits | un flottant sur 32 bits
* Faire `n` tours :
  * CMD : 0x02
  * Data : id du moteur sur 8 bits | 8 bits pour le nombre de tour, 8 bits pour le sens (0 ou 1)

### Moteur non asservis

* Tourner ON:
  * CMD : 0x03
  * Data : id du moteur sur 8 bit | sens rotation sur 8 bits (0 ou 1)
 
* Tourner OFF:
  * CMD : 0x04
  * Data : id du moteur sur 8 bit | sens rotation sur 8 bits (0 ou 1)

## Message servo

On prends l'implémentation qui a déjà été convenue
ID : 0x02

## Message IO

* Lire contacteur -> la carte réponds 0 ou 1 en data
  * ID : 0x03
  * CMD : 0x01
