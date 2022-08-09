from django.http.response import Http404
from django.shortcuts import get_object_or_404, render
from django.http import HttpResponse, HttpResponseRedirect
from django.urls import reverse
from django.shortcuts import redirect
from django.views import generic
from .models import Player, Item, File
from django.db.models import Q


#Landing page - displays all non alt characters above the guild parse min level
def index(request):
    players_list = Player.objects.filter(~Q(playerType='Alt')).order_by('playerName')
    context = {'player_list': players_list}
    return render(request, 'players/index.html', context)

#Detail page for the player
def detail(request, playerName):
    player = get_object_or_404(Player, playerName=playerName)
    items = Item.objects.filter(Q(winner=playerName)).order_by('itemDate')
    return render(request, 'players/detail.html', {'player': player, 'items' : items})

# Create your views here.
