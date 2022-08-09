from django.http.response import Http404
from django.shortcuts import get_object_or_404, render
from django.http import HttpResponse, HttpResponseRedirect
from django.urls import reverse
from django.shortcuts import redirect
from players.models import Player
from players.models import Item
from players.models import File

def index(request):
    return render(request, 'index.html')