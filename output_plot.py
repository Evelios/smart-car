import svgwrite

resolution = 50
size = (50, 50)

barriers = [ (150, 50), (200, 50), (550, 400), (150, 200), (50, 200), (250, 50), (50, 700), (650, 300), (550, 500), (400, 750), (100, 550), (600, 200), (350, 250), (250, 700), (700, 100), (600, 300), (500, 450), (150, 300), (200, 300), (200, 350), (50, 350), (50, 300), (300, 100), (350, 100), (300, 200), (400, 250), (300, 350), (250, 300), (200, 250), (100, 300), (350, 50), (300, 150), (400, 350), (50, 50) ]
solution = [ (0, 50), (0, 100), (0, 150), (0, 200), (0, 250), (0, 300), (0, 350), (0, 400), (50, 400), (100, 400), (150, 400), (200, 400), (250, 400), (300, 400), (350, 400) ]
start_pos = (0, 0)
goal_pos = (350, 400)


dwg = svgwrite.Drawing('Solution4.svg', profile='full')
dwg.add(dwg.rect(insert=(0, 0), size=('100%', '100%'), rx=None, ry=None, fill='rgb(50,50,50)'))

for point in barriers:
    dwg.add(dwg.rect(point, size, fill='rgb(198, 154, 141)'))

for point in solution:
    dwg.add(dwg.rect(point, size, fill='rgb(141, 198, 189)'))

dwg.add(dwg.rect(start_pos, size, fill='rgb(141, 198, 147)'))
dwg.add(dwg.rect(goal_pos, size, fill='rgb(141, 167, 198)'))

dwg.save()
