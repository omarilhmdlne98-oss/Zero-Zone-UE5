"""Run with UnrealEditor-Cmd -run=pythonscript -script=<absolute script path>. Use a fresh checkout."""
import math
import unreal as u

ROOT = '/Game/Theaters'
assets = u.EditorAssetLibrary
levels = u.get_editor_subsystem(u.LevelEditorSubsystem)
actors = u.get_editor_subsystem(u.EditorActorSubsystem)
tools = u.AssetToolsHelpers.get_asset_tools()
mel = u.MaterialEditingLibrary

def material(name, rgb, rough):
    p = ROOT + '/Surfaces'
    old = assets.load_asset(p + '/' + name) if assets.does_asset_exist(p + '/' + name) else None
    if old:
        return old
    m = tools.create_asset(name, p, u.Material, u.MaterialFactoryNew())
    tint = mel.create_material_expression(m, u.MaterialExpressionConstant3Vector, -320, 0)
    tint.set_editor_property('constant', u.LinearColor(*rgb, 1))
    mel.connect_material_property(tint, '', u.MaterialProperty.MP_BASE_COLOR)
    r = mel.create_material_expression(m, u.MaterialExpressionConstant, -320, 180)
    r.set_editor_property('r', rough)
    mel.connect_material_property(r, '', u.MaterialProperty.MP_ROUGHNESS)
    mel.recompile_material(m)
    if not assets.save_loaded_asset(m):
        raise RuntimeError('Could not save ' + name)
    return m

sand = material('M_Dune', (.53,.34,.15), .86)
silt = material('M_Silt', (.11,.075,.05), .32)
rock = material('M_Chalk', (.48,.46,.39), .82)
concrete = material('M_Cast', (.29,.28,.25), .72)
water = material('M_CanalWater', (.026,.13,.14), .14)
leaf = material('M_Frond', (.12,.21,.08), .88)
f = u.DataAssetFactory(); f.set_editor_property('data_asset_class', u.ZeroZoneTheaterPalette)
pp = ROOT + '/DA_TheaterPalette'
pal = assets.load_asset(pp) if assets.does_asset_exist(pp) else tools.create_asset('DA_TheaterPalette', ROOT, u.ZeroZoneTheaterPalette, f)
for key, default in [('wind_rippled_sand',sand),('wet_canal_silt',silt),('fractured_limestone',rock),('weathered_concrete',concrete)]:
    if not pal.get_editor_property(key): pal.set_editor_property(key, default)
if not assets.save_loaded_asset(pal, False): raise RuntimeError('Palette save failed')
concrete = pal.get_editor_property('weathered_concrete'); silt = pal.get_editor_property('wet_canal_silt')
cube = u.load_asset('/Engine/BasicShapes/Cube'); sphere = u.load_asset('/Engine/BasicShapes/Sphere')

def spawn(cls,x,y,z):
    a = actors.spawn_actor_from_class(cls, u.Vector(x,y,z))
    if not a: raise RuntimeError('Spawn failed: ' + str(cls))
    return a

def prop(label,x,y,z,sx,sy,sz,mat,mesh=cube,yaw=0):
    a = spawn(u.StaticMeshActor,x,y,z); a.set_actor_label(label)
    c = a.static_mesh_component; c.set_static_mesh(mesh); c.set_material(0,mat)
    a.set_actor_scale3d(u.Vector(sx,sy,sz)); a.set_actor_rotation(u.Rotator(yaw=yaw),False)
    return a

for hills, name in [(False,'AmberCanal'),(True,'ChalkReach')]:
    package = ROOT + '/' + name
    if assets.does_asset_exist(package): raise RuntimeError('Refusing to overwrite ' + package)
    if not levels.new_level(package, True): raise RuntimeError('Partitioned level creation failed')
    if not u.ZeroZoneTheaterLibrary.bake_theater_landscape(hills, pal): raise RuntimeError('Landscape import failed: ' + name)
    def h(x,y): return u.ZeroZoneTheaterLibrary.theater_height_cm(hills,x,y)
    x0,y0 = (10000,0) if hills else (6500,0)
    for i in range(4):
        x,y = x0+300*i,y0-2200
        spawn(u.PlayerStart,x,y,h(x,y)+160)
    opening = spawn(u.ZeroZoneBattleOpeningManager,x0,y0,h(x0,y0)); opening.set_editor_property('is_spatially_loaded',False)
    spawn(u.SkyAtmosphere,0,0,0)
    sun = spawn(u.DirectionalLight,0,0,10000); sun.light_component.set_mobility(u.ComponentMobility.MOVABLE)
    sun.light_component.set_editor_property('atmosphere_sun_light',True); sun.light_component.set_intensity(45000 if hills else 65000)
    sun.set_actor_rotation(u.Rotator(pitch=-30,yaw=-40),False)
    sky = spawn(u.SkyLight,0,0,0); sky.light_component.set_mobility(u.ComponentMobility.MOVABLE); sky.light_component.set_editor_property('real_time_capture', True)
    fog = spawn(u.ExponentialHeightFog,0,0,0); fc = fog.get_component_by_class(u.ExponentialHeightFogComponent)
    fc.set_editor_property('volumetric_fog', True); fc.set_editor_property('fog_density', .014 if hills else .022)
    if not hills:
        for j in range(-10,11): prop('Canal reach',0,j*19200,-180,76,192,1,water)
        for y in (-17000,21000):
            for i in range(-10,11): prop('Floating deck',i*620,y,h(6500,y)+100,6.2,6,.6,concrete)
        for j in range(32):
            x,y = -7400-1600*(j%3), (j-16)*2500
            z = h(x,y); prop('Palm stem',x,y,z+330,.9,.9,6.6,silt)
            for k in range(3): prop('Palm canopy',x,y,z+670,7,1.8,.9,leaf,sphere,k*60)
        for j in range(6):
            x,y = 9300+1100*(j%2),j*5300; z=h(x,y)
            prop('Concrete shelter',x,y,z+160,9,6,3.2,concrete)
            prop('Abandoned vehicle hull',x+1700,y,h(x+1700,y)+95,5,2.4,1.9,silt)
    else:
        for j in range(24):
            x,y = 14400+2100*(j%4),(j//4-3)*2800; z=h(x,y); tall=400+120*(j%3)
            prop('Courtyard block',x,y,z+tall/2,9,8,tall/100,concrete)
            prop('Courtyard wall',x+850,y,z+110,1,13,2.2,concrete)
        for j in range(4):
            x,y=12000,j*6500-10000; z=h(x,y)
            prop('Lookout pillar',x,y,z+550,2,2,11,concrete); prop('Lookout cabin',x,y,z+1150,5,5,2,concrete)
    if not levels.save_current_level(): raise RuntimeError('Level save failed: ' + name)
    if not u.EditorLoadingAndSavingUtils.save_dirty_packages(True,True): raise RuntimeError('External actor save failed')
    u.log('Built ' + package)
