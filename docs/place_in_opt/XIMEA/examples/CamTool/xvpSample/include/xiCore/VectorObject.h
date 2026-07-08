#ifndef VECTOROBJECT_H
#define VECTOROBJECT_H

#include <QFont>
#include <QGraphicsObject>
#include <xiCoreGlobal.h>

#define DX_HIT_DIST     6

//---------------------------------------------------------------------------
//! \brief Base class for all our vector objects in QGraphicsScene. Needed to send signals away.
class XICORE_API CxVectorObject : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit CxVectorObject(QGraphicsItem *parent = 0);

    TxVecObjID id();
    void setId(TxVecObjID idObj);

    CxVectorObject* clone();
    virtual void copyTo(CxVectorObject *pNewClone);
    virtual QString localizedObjectClassName();

    virtual void setPenColor(const QColor &color);
    virtual void setPenWidth(int iWidth);
    virtual void setSelectedPenWidth(int iWidth); //!< Set pen width of selected object
    virtual void setBrushColor(const QColor &color);
    virtual void setCoordIncrement(int nXInc, int nYInc);   //!< 0 for no limit, 1 for rounding to nearest int, 4 for coords in multiples of 4
    virtual void setResizable(bool bEnable);    //!< Enable object resizing with mouse
    virtual void setKeepInScene(bool bKeep);    //!< Forces the object to stay within image bounds
    virtual void scaleObject(double sx, double sy, bool bReflectConstraints = true);    //!< Object scaling
    virtual void moveObject(double dx, double dy, bool bReflectConstraints = true);     //!< Object translation

	static double distanceBetweenPointAndLine(const QPointF &pt, const QLineF &line);

    QColor penColor();
    QColor brushColor();
    int penWidth();
    int selectedPenWidth();//!< Width of a pen of selected object.

    enum ExObjectPlacement
    {
        explaceFree         = 0,    //!< User can more the object freely (default)
        explaceTopLeft      = 1,
        explaceTopCenter    = 2,
        explaceTopRight     = 3,
        explaceMidLeft      = 4,
        explaceMidCenter    = 5,
        explaceMidRight     = 6,
        explaceBottomLeft   = 7,
        explaceBottomCenter = 8,
        explaceBottomRight  = 9,
    };
    virtual void setScenePlacement(ExObjectPlacement ePlacement);
    ExObjectPlacement scenePlacement();

    enum ExZoomBehavior
    {
        exscaleWithZoom,    //!< Pen thickness and fint size increases as the image zoom increases, useful for user annotations
        exscaleFixed,       //!< Pen thickness and font size remains same even when image is zoomed, useful for image info, measurements, etc
    };
    virtual void setZoomBehavior(ExZoomBehavior eBehavior);
    ExZoomBehavior zoomBehavior();

public:
    enum ExCameraFormatChangeBehavior
    {
        excfcbAttachSensor,    //!< Keep same position on chip relative to ROI and binning (default)
        excfcbAttachScene,     //!< Stay visually on same place in the image view
        excfcbIgnore,          //!< No change when format change
    };
    ExCameraFormatChangeBehavior cameraFormatChangeBehavior();                      //!< See \ref ExCameraFormatChangeBehavior
    virtual void setCameraFormatChangeBehavior(ExCameraFormatChangeBehavior eMode); //!< See \ref ExCameraFormatChangeBehavior

public:
    enum ExUsageFlag
    {
        exusageAnnotation   = 1,    //!< Is Annotation object (may ba saved to file)
        exusageMeasurement  = 2,    //!< Is measurement object
    };
    void setObjectUsageFlag(ExUsageFlag eUsage, bool bSet = true);
    bool testObjectUsage(ExUsageFlag eUsage);

signals:
    void customContextMenuRequested(const QPoint &pos); //!< Sent after right mouse click on the object
    void doubleClicked(int idObject);
    void clicked(int idObject);

public slots:
    void updatePosForFixedScenePlacement();

protected:
    TxVecObjID m_id;        //!< ID as set by CxVecObjContainer
    QColor m_clPen;         //!< Color of the pen used when painting the object
    QColor m_clBrush;       //!< Fill color
    int m_iPenWidth;
    int m_iSelectedPenWidth; //!< Pen width of selected object
    int m_nCoordXIncrement; //!< 0 for no limit, 1 for rounding to nearest int, 4 for coords in multiples of 4
    int m_nCoordYIncrement;
    bool m_bIsResizable;    //!< It is possible to resize the object (rectangle, line end points, ...)
    bool m_bKeepInScene;    //!< Do not allow to move the object outside the image boundaries
    ExObjectPlacement m_eScenePlacement;   //!< Fixed object position inside the scene (default: explaceFree)
    ExZoomBehavior m_eZoomBehavior;
    ExCameraFormatChangeBehavior m_eCameraFormatChangeBehavior; //!< Defaults to \ref excfcbAttachSensor
    QFlags<ExUsageFlag> m_flagsObjectUsage;

    QPen defaultPen() const;
    QBrush defaultBrush() const;
    bool reflectCoordIncrement(QPointF& pt) const;
    qreal viewScaleAtEvent(QGraphicsSceneEvent *pEvent);

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

//---------------------------------------------------------------------------
//! \brief Object representing the rectangular object. Implements mouse resizing and caption.
class XICORE_API CxVectorObjectRect : public CxVectorObject
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit CxVectorObjectRect(CxVectorObject *parent = 0);

public:
    QRectF rect();
    void setRect(const QRectF &rect);
    void setRect(qreal x, qreal y, qreal w, qreal h);
    qreal width() const;
    qreal height() const;
    void setWidth(qreal dWidth);
    void setHeight(qreal dHeight);
    void setSizeIncrement(int nWidthInc, int nHeightInc);   //!< 0 for no limit, 1 for rounding to nearest int, 4 for coords in multiples of 4
    void setMinSize(qreal dMinWidth, qreal dMinHeight);     //!< Minimal size of the rectangle
    QString caption();                          //!< Caption is text drawn in the center of rectangle
    virtual void setCaption(const QString &sCaption);
    QColor captionColor();
    void setCaptionColor(const QColor &color);
    QFont captionFont();
    virtual void setCaptionFont(const QFont &aFont);

protected:
    virtual void drawObjectContents(QPainter *painter, double dImageViewScale = 1.0);
    virtual void drawCaption(QPainter *painter, const QRectF &rcIn, double dImageViewScale = 1.0);

    // from CxVectorObject
public:
    QString localizedObjectClassName() override;
    void copyTo(CxVectorObject *pNewClone) override;
    void scaleObject(double sx, double sy, bool bReflectConstraints = true) override;
    void moveObject(double dx, double dy, bool bReflectConstraints = true) override;

signals:
    void rectChanging(const QRectF &rc);    //!< Object is being changed (sent during mouse moves)
    void rectChanged(const QRectF &rc);     //!< Object has new geometry (sent after mouse up)

public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *widget) override;
    QRectF boundingRect() const override;
public:
    enum ExRectShape
    {
        ersSharpCorners,
        ersRoundCorners
    };
    ExRectShape rectShape() const;
    void setRectShape(ExRectShape eShape);
    double cornerRadiusX() const;
    double cornerRadiusY() const;
    Qt::SizeMode cornerSizeMode() const;
    void setCornersRadius(double dXradius, double dYradius, Qt::SizeMode mode = Qt::AbsoluteSize);
    void setCornersRadius(double dRadius, Qt::SizeMode mode = Qt::AbsoluteSize);
protected:
    ExRectShape m_eShape;
    double m_dXradius;
    double m_dYradius;
    Qt::SizeMode m_eCornerSizeMode;

protected:
    qreal m_dWidth, m_dHeight;
    enum ExRectHitTest
    {
        ehtNone,
        ehtMove,
        ehtTopEdge, ehtLeftEdge, ehtRightEdge, ehtBottomEdge,
        ehtTLCorner, ehtTRCorner, ehtBLCorner, ehtBRCorner,
    } m_eLastHitTest;
    int m_nWidthIncrement, m_nHeightIncrement;
    QSizeF m_sizeMin;
    QString m_sCaption;
    QColor m_clCaption;
    QFont m_aCaptionFont;
    bool m_bHasCustomCaptionFont;

    bool m_bResizing;   // currently resizing using m_eLastHitTest
    QPointF m_ptStartDragMousePos, m_ptStartRectPos;    // helper members for dragging

    virtual ExRectHitTest hitTest(qreal x, qreal y, qreal dViewScale);
    bool reflectSizeIncrement(qreal &w, qreal &h) const;

protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

//---------------------------------------------------------------------------
//! \brief Object representing the single line, from start point to end point.
class XICORE_API CxVectorObjectLine : public CxVectorObject
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit CxVectorObjectLine(CxVectorObject *parent = 0);

public:
    QLineF line();                      //!< Returns line geometry
    void setLine(const QLineF &line);   //!< Sets line geometry
    void setPadding(int padding);		//!< Sets a minimal padding of the line from the scene edge
    QString localizedObjectClassName() override;
    void copyTo(CxVectorObject *pNewClone) override;
    void setCoordIncrement(int nXInc, int nYInc) override;   //!< Reimplemented to check both end points
    void scaleObject(double sx, double sy, bool bReflectConstraints = true) override;
    void moveObject(double dx, double dy, bool bReflectConstraints = true) override;

    enum ExLineStyle
    {
        elsFree,
        elsHorizontal,  //!< Force line to be horizontal
        elsVertical,    //!< Force line to be vertical
    };

    ExLineStyle lineStyle();
    void setLineStyle(ExLineStyle eStyle);

signals:
    void lineChanging(const QLineF &line);    //!< Object is being changed (sent during mouse moves)
    void lineChanged(const QLineF &line);     //!< Object has new geometry (sent after mouse up)

public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

protected:
    QPointF m_ptEnd;
    ExLineStyle m_eLineStyle;
	int m_iPadding;

    enum ExLineHitTest
    {
        ehtNone,
        ehtEdge,
        ehtStartPoint, ehtEndPoint
    } m_eLastHitTest;

    bool m_bResizing;   // currently resizing using m_eLastHitTest
    QPointF m_ptStartDragMousePos, m_ptStartPos;    // helper members for dragging

    double distanceFromPoint(const QPointF &pt) const;

protected:
    virtual ExLineHitTest hitTest(qreal x, qreal y, qreal dViewScale);
    QRectF boundingRect() const override;

protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
};

//---------------------------------------------------------------------------
//! \brief Object representing the ellipse, using the functionality implemented in rectangle.
class XICORE_API CxVectorObjectEllipse : public CxVectorObjectRect
{
	Q_OBJECT
public:
    Q_INVOKABLE explicit CxVectorObjectEllipse(CxVectorObject *parent = 0);
    void setCenterAndRadius(const QPointF &ptCenter, qreal dRadiusX, qreal dRadiusY);

    // from CxVectorObject
public:
    QString localizedObjectClassName() override;

    // from CxVectorObjectRect
protected:
    void drawObjectContents(QPainter *painter, double dImageViewScale = 1.0) override;
};

//---------------------------------------------------------------------------
//! \brief Object representing the polygon, not having any interaction possibilities yet
class XICORE_API CxVectorObjectPolygon : public CxVectorObject
{
	Q_OBJECT
public:
    Q_INVOKABLE explicit CxVectorObjectPolygon(CxVectorObject *parent = 0);
    void setPointCount(int nCount);
    void setPoint(int iPointIdx, const QPointF &ptCoord);

    // from CxVectorObject
public:
    QString localizedObjectClassName() override;
    void copyTo(CxVectorObject *pNewClone) override;

public:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) override;

protected:
    QRectF boundingRect() const override;
private:
    QVector<QPointF> m_pts;
    QPolygonF m_polyToDraw;
};

//---------------------------------------------------------------------------
//! \brief Object representing the text. Use setCaption to set the text
class XICORE_API CxVectorObjectText : public CxVectorObjectRect
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit CxVectorObjectText(CxVectorObject *parent = 0);

    // from CxVectorObject
public:
    QString localizedObjectClassName() override;
    void setCaption(const QString &sCaption) override;
    void setCaptionFont(const QFont &aFont) override;

protected:
    virtual void updateSizeToCaption();
};

//---------------------------------------------------------------------------
//! \brief Object representing the scale bar with text label
class XICORE_API CxVectorObjectScaleBar : public CxVectorObjectText
{
    Q_OBJECT
public:
    Q_INVOKABLE explicit CxVectorObjectScaleBar(CxVectorObject *parent = 0);

protected:
    qreal m_dScaleBarLength;    //!< scale bar length in calibrated unit
    int m_iLengthUnit;          //!< scale bar length unit
    bool m_bScaleBarLengthAuto; //!< calculate \ref m_dScaleBarLength so that is looks good for current image width

    qreal m_dConversionFactor;  //!< in length per pixel
    int m_iConversionFactorUnit;//!< unit of \ref m_dConversionFactor

    qreal m_dScaleBarLengthPx;  //!< resulting size in image pixels, calculated in \ref calculateScaleBarLength()

    void calculateScaleBarLength();

    // from CxVectorObject
public:
    QString localizedObjectClassName() override;
    void copyTo(CxVectorObject *pNewClone) override;

public:
    void setConversionFactor(qreal dLength, int iUnit);     //!< set pixel size (conversion factor)
    void conversionFactor(qreal *pdLength, int *piUnit);    //!< get pixel size (conversion factor)
    void setScaleBarLength(qreal dLength, int iUnit, bool bAutoLengthAndUnit);      //!< set scale bar length in calibrated unit
    void scaleBarLength(qreal *pdLength, int *piUnit, bool *pbAutoLengthAndUnit);   //!< get scale bar length in calibrated unit

protected:
    void drawObjectContents(QPainter *painter, double dImageViewScale = 1.0) override;
    void updateSizeToCaption() override;
};

#endif // VECTOROBJECT_H
