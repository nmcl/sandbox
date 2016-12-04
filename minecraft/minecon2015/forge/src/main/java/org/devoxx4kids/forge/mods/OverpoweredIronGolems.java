package org.devoxx4kids.forge.mods;

import net.minecraft.entity.EntityLiving;
import net.minecraft.entity.monster.EntityIronGolem;
import net.minecraft.potion.PotionEffect;
import net.minecraftforge.event.entity.EntityJoinWorldEvent;
import net.minecraftforge.fml.common.eventhandler.SubscribeEvent;

public class OverpoweredIronGolems {

	@SubscribeEvent
	public void golemMagic(EntityJoinWorldEvent event) {
		if (!(event.entity instanceof EntityIronGolem)) {
			return;
		}

		EntityLiving entity = (EntityLiving) event.entity;
		entity.addPotionEffect(new PotionEffect(1, 1000000, 5));
		entity.addPotionEffect(new PotionEffect(5, 1000000, 5));
		entity.addPotionEffect(new PotionEffect(10, 1000000, 5));
		entity.addPotionEffect(new PotionEffect(11, 1000000, 5));
	}

}