from django.contrib import admin

from .models import Player
from .models import Item
from .models import File

admin.site.register(Player)
admin.site.register(Item)
admin.site.register(File)