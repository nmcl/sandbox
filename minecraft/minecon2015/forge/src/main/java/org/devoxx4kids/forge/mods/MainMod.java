package org.devoxx4kids.forge.mods;

import net.minecraft.enchantment.Enchantment;
import net.minecraft.util.ResourceLocation;
import net.minecraftforge.common.MinecraftForge;
import net.minecraftforge.fml.common.FMLCommonHandler;
import net.minecraftforge.fml.common.Mod;
import net.minecraftforge.fml.common.Mod.EventHandler;
import net.minecraftforge.fml.common.event.FMLInitializationEvent;

@Mod(modid = MainMod.MODID, version = MainMod.VERSION)
public class MainMod {
	public static final String MODID = "MyMods";
	public static final String VERSION = "1.0";
	
	public static final Enchantment haste = new EnchantmentArrowFast(150, new ResourceLocation("haste"), 2);

	@EventHandler
	public void init(FMLInitializationEvent event) {
		MinecraftForge.EVENT_BUS.register(new ChatItems());
		MinecraftForge.EVENT_BUS.register(new DragonSpawner());
		MinecraftForge.EVENT_BUS.register(new SquidSpawner());
		MinecraftForge.EVENT_BUS.register(new CreeperSpawnAlert());
		MinecraftForge.EVENT_BUS.register(new SharpSnowballs());
		MinecraftForge.EVENT_BUS.register(new OverpoweredIronGolems());
		MinecraftForge.EVENT_BUS.register(new RainWater());
		
		FMLCommonHandler.instance().bus().register(new WallClimber());
		MinecraftForge.EVENT_BUS.register(new WallClimber());
		
		MinecraftForge.EVENT_BUS.register(new ArrowShotgun());
		MinecraftForge.EVENT_BUS.register(new SkeletonWar());
		
		Enchantment.addToBookList(haste);
		MinecraftForge.EVENT_BUS.register(haste);
	}
}