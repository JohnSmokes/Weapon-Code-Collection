// Created by Crowbar 0.71

$modelname "v_autoass.mdl"
$cd "."
$cdtexture "."
$cliptotextures
$scale 1.7

$bodygroup "hands"
{
	studio "hands"
}
$bodygroup "studio"
{
	studio "AA12-1"
}
$bodygroup "studio"
{
	studio "AA12-2"
}
$bodygroup "studio"
{
	studio "AA12-3"
}

$flags 0


$attachment 0 "Muzzle" 0 0 0

$cbox 0 0 0 0 0 0

$bbox 0 0 0 0 0 0

$hbox 0 "v_weapon.Hands_parent" -4.779829 -1.30185 -6.815648 6.710234 1.415756 0
$hbox 0 "v_weapon.Left_Arm" -1.56 -1.23 -8.559245 1.76 1.56 0.26
$hbox 0 "v_weapon.Root16" -4.292839 -8.162795 -2.950983 0 0 0
$hbox 0 "v_weapon.Left_Hand" -1.91 -1.03 -0.26 1.32 1.5 3.18
$hbox 0 "v_weapon.Left_Thumb_02" -0.52 -0.47 0 0.49 0.61 1.02
$hbox 0 "v_weapon.L_wrist_helper" -0.86 -1.02 0 1.63 0.8 2.38
$hbox 0 "v_weapon.Right_Arm" -1.36 -1.65 -8.5416 1.42 1.71 0.42
$hbox 0 "v_weapon.Right_Hand" -0.91 -1.76 -0.19 0.9 1.44 3.1
$hbox 0 "v_weapon.Right_Thumb01" -0.64 -0.65 -0.34 0.85 0.88 1.978469
$hbox 0 "v_weapon.Right_Thumb02" -0.49 -0.57 0 0.61 0.57 1.03
$hbox 0 "v_weapon.R_wrist_helper" -1.1 -1.23 -0.04 0.93 1.31 2.52
$hbox 0 "USAS" -0.96 -2.78 -21.456202 0.79 5.95 10.96
$hbox 0 "Mag" -3.15 -3.14 -1.6 2.77 3.72 1.5

$sequence "idle" {
	"v_autoass_anims\idle"
	fps 12
}
$sequence "shoot1" {
	"v_autoass_anims\shoot1"
	{ event 5001 0 "21" }
	fps 40
}
$sequence "shoot2" {
	"v_autoass_anims\shoot2"
	{ event 5001 0 "21" }
	fps 40
}
$sequence "insert" {
	"v_autoass_anims\insert"
	fps 90
}
$sequence "after_reload" {
	"v_autoass_anims\after_reload"
	{ event 5004 10 "items/clipinsert1.wav" }
	fps 30
}
$sequence "start_reload" {
	"v_autoass_anims\start_reload"
	{ event 5004 10 "items/cliprelease1.wav" }
	{ event 5004 26 "items/clipinsert1.wav" }
	fps 54
}
$sequence "draw" {
	"v_autoass_anims\draw"
	{ event 5004 1 "weapons/gunpickup4.wav" }
	fps 22
}
